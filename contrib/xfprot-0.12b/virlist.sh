#!/bin/sh
/usr/local/f-prot/f-prot -virlist 2>/dev/null|less 2>/dev/null ||
/usr/local/f-prot/f-prot -virlist 2>/dev/null | more ||
/usr/local/f-prot/f-prot -virlist 2>/dev/null
echo ""
echo -n "Press Enter to close the console  "
read $dummy
