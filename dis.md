gcc -O0 -g -o float_add float_add.c
objdump -d -M intel -S float_add | grep -A20 '<add_floats>:'

