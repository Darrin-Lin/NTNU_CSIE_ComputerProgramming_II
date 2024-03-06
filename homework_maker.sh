#!/bin/sh
echo "hw?(ex:hw01 ==> 1)"
read number
mkdir "./Homework${number}"
mkdir "./Homework${number}/HW0${number}"
mkdir "./Homework${number}/test_hw0${number}"
for i in $(seq 1 5)
do
	cp "./hw_making_tools/hw_c_example.c" "./Homework${number}/HW0${number}/hw0${number}0${i}.c"
	cp "./hw_making_tools/hw_test_in_file_example.py" "./Homework${number}/test_hw0${number}/test0${number}0${i}.py"
	sed -i "s/hw_part=1/hw_part=${i}/g" "./Homework${number}/test_hw0${number}/test0${number}0${i}.py"
	sed -i "s/hw=1/hw=${number}/g" "./Homework${number}/test_hw0${number}/test0${number}0${i}.py"

done
cp "./hw_making_tools/hw_makefile_example" "./Homework${number}/HW0${number}/Makefile"
cp "./hw_making_tools/hw_README_example.md" "./Homework${number}/HW0${number}/README.md"
cp "./hw_making_tools/hw_test_out_file_example.sh" "./Homework${number}/test_hw0${number}/hw0${number}.sh"
cp "./hw_making_tools/hw_test_remove.sh" "./Homework${number}/test_hw0${number}/remove.sh"
sed -i "s/hw=1/hw=${number}/g" "./Homework${number}/test_hw0${number}/hw0${number}.sh"
sed -i "s/hw_number = 0/hw_number = ${number}/g" "./Homework${number}/HW0${number}/Makefile"
sed -i "s/hw0/hw0${number}/g" "./Homework${number}/HW0${number}/README.md"
sed -i "s/HW0/HW0${number}/g" "./Homework${number}/HW0${number}/README.md"