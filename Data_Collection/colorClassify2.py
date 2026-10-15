import serial
import time
import numpy as np
import tensorflow as tf
from tensorflow import keras
import joblib

# =========================================================
#  Color Classifier (Neural Network + Arduino Serial Input)
# =========================================================

class ColorClassifier:
    def __init__(self, model_path, scaler_path):
        # Load trained model and scaler
        self.model = keras.models.load_model(model_path)
        self.scaler = joblib.load(scaler_path)

        self.label_encoder = joblib.load('uno_label_encoder.pkl')
        self.color_labels = list(self.label_encoder.classes_)

        print("✓ Model and scaler loaded successfully")
        print(f"✓ Number of classes: {len(self.color_labels)}")

    def predict_color(self, rgb_values):
        """Predict color from raw RGB values"""
        rgb_array = np.array(rgb_values).reshape(1, -1)
        rgb_scaled = self.scaler.transform(rgb_array)

        predictions = self.model.predict(rgb_scaled, verbose=0)
        predicted_class = np.argmax(predictions[0])
        confidence = predictions[0][predicted_class]
        return self.color_labels[predicted_class], confidence, predictions[0]

    def print_detailed_predictions(self, predictions, top_n=3):
        """Print top N predictions with confidence scores"""
        top_indices = np.argsort(predictions)[-top_n:][::-1]
        print("Top predictions:")
        for i, idx in enumerate(top_indices):
            confidence = predictions[idx] * 100
            print(f"  {i+1}. {self.color_labels[idx]:15} {confidence:5.1f}%")


# =========================================================
#  Serial Communication
# =========================================================
def read_serial_data(ser):
    """Continuously wait for a valid RGB line from Arduino."""
    print("📡 Waiting for RGB data from Arduino...")
    while True:
        try:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if not line:
                continue

            print("RAW:", repr(line))  # Debug line to confirm data arrival

            # Expect "R,G,B"
            if ',' in line:
                parts = [p.strip() for p in line.split(',')]
                if len(parts) == 3 and all(p.isdigit() for p in parts):
                    r, g, b = map(int, parts)
                    print(f"✅ Received RGB = {r}, {g}, {b}")
                    return r, g, b

            elif "READY" in line.upper():
                print("✓ Arduino reports ready.")

        except KeyboardInterrupt:
            raise
        except Exception as e:
            print(f"⚠️ Serial read error: {e}")
        time.sleep(0.05)


# =========================================================
#  Main Loop
# =========================================================
def main():
    PORT = 'COM4'           # 🔧 Change to your Arduino port
    BAUD_RATE = 115200
    MODEL_PATH = 'uno_color_classifier.h5'
    SCALER_PATH = 'uno_scaler.pkl'

    try:
        # Initialize model + scaler
        classifier = ColorClassifier(MODEL_PATH, SCALER_PATH)

        # Connect to Arduino
        print(f"🔌 Connecting to Arduino on {PORT} ...")
        ser = serial.Serial(PORT, BAUD_RATE, timeout=2, rtscts=False, dsrdtr=False)
        ser.rts = False
        ser.dtr = False
        time.sleep(2)
        ser.reset_input_buffer()
        print(f"✓ Connected successfully to {PORT}")

        print("\n" + "=" * 55)
        print("       REAL-TIME COLOR CLASSIFICATION (NEURAL NETWORK)       ")
        print("=" * 55)
        print("Instructions:")
        print("1️⃣ Place object under sensor")
        print("2️⃣ Press the Arduino button (if required)")
        print("3️⃣ View predictions below\n")

        sample_count = 0
        while True:
            print(f"\n🎯 Sample #{sample_count + 1} - waiting for measurement...")
            r, g, b = read_serial_data(ser)

            print(f"📊 RGB values received: R={r}, G={g}, B={b}")
            predicted_color, confidence, all_predictions = classifier.predict_color([r, g, b])

            print("\n🎨 CLASSIFICATION RESULT:")
            print(f"   Predicted Color : {predicted_color.upper()}")
            print(f"   Confidence      : {confidence * 100:.1f}%")

            classifier.print_detailed_predictions(all_predictions, top_n=3)

            if confidence < 0.6:
                print("⚠️  Low confidence - result may be unreliable")

            print("-" * 55)
            sample_count += 1
            time.sleep(0.5)

    except serial.SerialException as e:
        print(f"❌ Could not open {PORT}: {e}")
        print("➡ Check if Arduino is connected and no other program (e.g., Serial Monitor) is using the port.")
    except FileNotFoundError:
        print(f"❌ Missing model or scaler file. Make sure '{MODEL_PATH}' and '{SCALER_PATH}' exist.")
    except KeyboardInterrupt:
        print("\n🏁 Stopped by user.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("🔒 Serial port closed. Goodbye!")


# =========================================================
if __name__ == "__main__":
    main()
