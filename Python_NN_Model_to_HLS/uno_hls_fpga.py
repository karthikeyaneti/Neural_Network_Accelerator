import hls4ml
from tensorflow import keras
import os

def convert_to_hls(model_path='uno_color_classifier.h5'):
    try:
        # Check if model file exists
        if not os.path.exists(model_path):
            raise FileNotFoundError(f"Model file {model_path} not found!")
        
        print("Loading model from:", model_path)
        model = keras.models.load_model(model_path)
        
        print("Model loaded successfully!")
        print("Model summary:")
        model.summary()
        
        # Configure hls4ml
        print("\nConfiguring hls4ml...")
        config = hls4ml.utils.config_from_keras_model(model, granularity='name')
        
        config['Model']['Precision'] = 'ap_fixed<12,6>'
        config['Model']['ReuseFactor'] = 256  # Very high for maximum sharing
        config['Model']['Strategy'] = 'Resource'

        # Force BRAM usage for weights
        config['LayerName']['dense']['Storage'] = 'BRAM'
        config['LayerName']['dense_1']['Storage'] = 'BRAM'
        config['LayerName']['dense_2']['Storage'] = 'BRAM' 
        config['LayerName']['dense_3']['Storage'] = 'BRAM'

        # Use bind_op instead of resource pragma (fixes the warning)
        config['LayerName']['dense']['BindOp'] = 'shared'
        config['LayerName']['dense_1']['BindOp'] = 'shared'
        config['LayerName']['dense_2']['BindOp'] = 'shared'
        config['LayerName']['dense_3']['BindOp'] = 'shared'

        # Limit parallelization
        config['LayerName']['dense']['ParallelizationFactor'] = 1
        config['LayerName']['dense_1']['ParallelizationFactor'] = 1
        config['LayerName']['dense_2']['ParallelizationFactor'] = 1
        config['LayerName']['dense_3']['ParallelizationFactor'] = 1

        # Strategy - use Latency for better accuracy
        config['Model']['Strategy'] = 'Latency'

        # Keep all outputs for accuracy, we'll handle argmax in PS
        config['Model']['Output'] = 'all'

        # Batch normalization - keep for accuracy
        for bn_layer in ['batch_normalization', 'batch_normalization_1', 'batch_normalization_2']:
            config['LayerName'][bn_layer]['Strategy'] = 'Latency'
            
        config['Backend'] = 'Vivado'
        config['XilinxPart'] = 'xc7z010clg400-1'  # Zybo Z7-10 part
        config['ClockPeriod'] = 10  # 100 MHz clock
        config['IOType'] = 'io_parallel'  # Use parallel I/O for better performance
        
        # Strategy for resource optimization
        config['Model']['Strategy'] = 'Resource'
        
        print("hls4ml configuration:")
        for key, value in config.items():
            print(f"  {key}: {value}")
        
        # Convert the model
        print("\nConverting model to HLS...")
        hls_model = hls4ml.converters.convert_from_keras_model(
            model, 
            hls_config=config,
            output_dir='hls4ml_color_classifier',
            part='xc7z010clg400-1'
        )
        
        print("HLS model created successfully!")
        
        # Compile and build
        print("Compiling HLS model...")
        hls_model.compile()
        
        print("Building HLS project...")
        hls_model.build(csim=False, synth=True, vsynth=True)
        
        print("\n=== HLS Conversion Complete ===")
        print(f"HLS project location: hls4ml_color_classifier")
        print(f"FPGA Part: xc7z010clg400-1 (Zybo Z7-10)")
        
        # Print resource utilization estimate
        print("\nResource utilization report:")
        hls4ml.report.read_vivado_report('hls4ml_color_classifier')
        
        return hls_model
        
    except Exception as e:
        print(f"Error during HLS conversion: {e}")
        return None

# Run the conversion
hls_model = convert_to_hls()

if hls_model is not None:
    print("\nReady for Vivado integration!")
    print("Next steps:")
    print("1. Check the hls4ml_color_classifier directory for generated files")
    print("2. Use Vivado HLS to synthesize the IP core")
    print("3. Import the IP into your Vivado project")