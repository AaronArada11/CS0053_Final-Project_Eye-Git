set pagination off
set confirm off
set print pretty on
break debug/confirmation_fixed.cpp:30
run debug/eof_input.txt
printf "\n--- Closed input: first loop iteration ---\n"
print attempt
print answer
print inputEof
print inputFailed
print inputBad
bt
next
next
continue
