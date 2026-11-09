#!/bin/bash
echo "X5O!P%@AP[4\PZX54(P^)7CC)7}\$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!\$H+H*" > $HOME/f-prot-test-file
/usr/local/f-prot/f-prot $HOME/f-prot-test-file -nosub -report=$HOME/f-prot-test.log
echo ""
echo -n "Press Enter to close the console  "
read $dummy
rm -f $HOME/f-prot-test-file
