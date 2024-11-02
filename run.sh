#!/bin/bash

# Output file
OUTPUT_FILE="output.txt"

# Clear previous content if the file exists
if [ -f "$OUTPUT_FILE" ]; then
    echo "" > "$OUTPUT_FILE"  # Clear the content but keep the file
else
    touch "$OUTPUT_FILE"       # Create the file if it doesn't exist
fi

# Function to compile and run a scheduler
run_scheduler() {
    local scheduler_name=$1
    local schedule_file=$2

    echo -e "\n# compile and output $scheduler_name" >> "$OUTPUT_FILE"
    make "$scheduler_name" > /dev/null 2>&1  # Suppress output of make
    ./"$scheduler_name" "$schedule_file" >> "$OUTPUT_FILE" 2>&1  # Capture output of execution
}

# Run each scheduler
run_scheduler "fcfs" "schedule.txt"
run_scheduler "sjf" "schedule.txt"
run_scheduler "priority" "schedule.txt"
run_scheduler "rr" "schedule.txt"
run_scheduler "priority_rr" "schedule.txt"

echo "All scheduling algorithms have been executed. Check $OUTPUT_FILE for output."
