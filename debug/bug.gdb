set pagination off
set confirm off
set print pretty on
break debug/confirmation_bug.cpp:30
run debug/eof_input.txt
printf "\n--- Closed input: first loop iteration ---\n"
print attempt
print answer
print inputEof
print inputFailed
print inputBad
bt
continue
printf "\n--- Same breakpoint reached again without new input ---\n"
print attempt
print answer
print inputEof
print inputFailed
continue
printf "\n--- Third iteration: failed stream still retried ---\n"
print attempt
print inputEof
print inputFailed
kill
