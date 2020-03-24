
all clean:
	for dir in avr src; do (cd $$dir && ${MAKE} $@); done
