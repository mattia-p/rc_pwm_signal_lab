import serial
import time
import csv
import argparse
import sys
import os

# Set up argument parser
parser = argparse.ArgumentParser(description='Log PWM data from serial port to CSV file')
parser.add_argument('filename', help='Output CSV filename')
args = parser.parse_args()

# Ensure filename has .csv extension
if not args.filename.lower().endswith('.csv'):
    args.filename += '.csv'

# Set serial port and baud rate
PORT = '/dev/ttyACM0'
BAUD = 9600
LOG_DURATION_SEC = 60  # Logging time

try:
    # Open serial connection
    ser = serial.Serial(PORT, BAUD, timeout=1)
    
    # Wait longer for Arduino to reset and stabilize
    print("Waiting for Arduino to initialize...")
    time.sleep(3)  # Increased from 2 to 3 seconds
    
    # Clear any initial garbage data
    ser.reset_input_buffer()
    
    with open(args.filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['steering', 'throttle', 'engage'])

        start = time.time()
        print(f"Logging started. Press Ctrl+C to stop early...")

        # Skip first few readings to avoid reset artifacts
        for _ in range(5):  # Skip first 5 readings
            ser.readline()

        while time.time() - start < LOG_DURATION_SEC:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line:
                try:
                    values = list(map(int, line.split(',')))
                    if len(values) == 3:
                        # Add basic validation
                        # if all(1000 <= v <= 2000 for v in values):  # Typical PWM range
                        writer.writerow(values)
                        # else:
                        #     print(f"Skipping out-of-range values: {values}")
                except ValueError:
                    pass  # Skip malformed lines

except KeyboardInterrupt:
    print("\nLogging interrupted by user.")

except serial.SerialException as e:
    print(f"Serial port error: {e}")
    sys.exit(1)

finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
    print(f"Data logging finished. Saved to {args.filename}")
