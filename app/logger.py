from datetime import datetime, timezone

def write_log(msg, file_path="logs/arduino.log") :
    timestamp = datetime.now().astimezone(timezone.utc)

    with open(file_path, 'a') as f :    # Writing at the end of the log file
        f.write(f"{timestamp.isoformat()} - {msg}\n") 


