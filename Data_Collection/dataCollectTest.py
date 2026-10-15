# color_data_collector_batch.py
import serial
import csv
import time
from datetime import datetime

class ColorDataCollector:
    def __init__(self, port='COM4', baudrate=115200, timeout=1):
        # Configure serial without hardware flow control
        self.ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            timeout=timeout,
            rtscts=False,    # Disable RTS/CTS
            dsrdtr=False     # Disable DSR/DTR
        )
        
        # Immediately disable RTS and DTR to prevent ESP32 reset
        self.ser.rts = False
        self.ser.dtr = False
        
        time.sleep(2)  # Wait for ESP32 to stabilize
        
        self.data_file = f"color_dataset_{datetime.now().strftime('%Y%m%d_%H%M')}.csv"
        
        print("ESP32 connected. Starting data collection...")
        
        # Initialize CSV file
        with open(self.data_file, 'w', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(['red', 'green', 'blue', 'label'])
        
        print(f"Data will be saved to: {self.data_file}")
        print("Manually reset ESP32 if needed, then press Enter to continue...")
        input()

    def read_rgb_data(self):
        """Read RGB data with simple polling"""
        while True:
            if self.ser.in_waiting > 0:
                try:
                    line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                    
                    # Look for data pattern: number,number,number
                    if line and ',' in line:
                        parts = line.split(',')
                        if len(parts) == 3:
                            try:
                                r = int(parts[0].strip())
                                g = int(parts[1].strip())
                                b = int(parts[2].strip())
                                
                                if all(0 <= x <= 4095 for x in [r, g, b]):
                                    return r, g, b
                            except ValueError:
                                continue
                except Exception:
                    continue
            time.sleep(0.01)

    def collect_batch_samples(self):
        """Simple data collection without complex handshaking"""
        colors = ['red', 'green', 'blue', 'yellow', 'teal', 'orange', 'purple', 'pink', 'white', 'black']
        colors.sort()
        samples_per_color = 50  # Reduced for testing
        
        print("=== DATA COLLECTION ===")
        print("Instructions:")
        print("1. Place color object under sensor")
        print("2. Press ESP32 button for each sample")
        print("3. Press Ctrl+C to stop\n")
        
        for color in colors:
            print(f"\n>>> Collecting {color.upper()} <<<")
            input(f"Place {color} object and press Enter to start...")
            
            print(f"Collecting {samples_per_color} samples. Press ESP32 button for each.")
            
            samples = []
            collected = 0
            
            while collected < samples_per_color:
                try:
                    r, g, b = self.read_rgb_data()
                    samples.append([r, g, b])
                    collected += 1
                    print(f"  Sample {collected}: R={r}, G={g}, B={b}")
                    
                    # Save immediately
                    with open(self.data_file, 'a', newline='') as f:
                        writer = csv.writer(f)
                        writer.writerow([r, g, b, color])
                        
                except KeyboardInterrupt:
                    print(f"\nStopped early. Collected {collected} samples for {color}.")
                    break
            
            print(f"✓ {color}: {collected} samples")
        
        print(f"\n=== COMPLETE ===")
        print(f"Data saved to: {self.data_file}")

    def close(self):
        if self.ser and self.ser.is_open:
            self.ser.close()

if __name__ == "__main__":
    collector = None
    try:
        collector = ColorDataCollector(port='COM4')  # Change to your port
        collector.collect_batch_samples()
    except Exception as e:
        print(f"Error: {e}")
    finally:
        if collector:
            collector.close()