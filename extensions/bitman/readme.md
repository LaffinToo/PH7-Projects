# BitMan

A bit manipulation library. 
Function constants defined: 

**bitman(*function*,...)**

-**BIT_AND** *bitman(BIT_AND, $val1, $val1)*
returns $val1 AND $val2

-**BIT_OR** *bitman(BIT_OR, $val1, $val2)*
returns $val1 OR $val2

-**BIT_XOR** *bitman(BIT_XOR, $val1, $val2)*
returns $val1 XOR $val2

-**BIT_ROL** *bitman(BIT_ROL, $val, $bits, $maxbits)*
returns $val rotated left by $bits, left bits get shifted to right. $maxbits limits the rotate to specified number of bits. 

-**BIT_ROR** *bitman(BIT_ROR, $val, $bits, $maxbits)*
returns $val rotated right by $bits, right bits get shifted to left. $maxbits limits the rotate to specified number of bits. 

-**BIT_STRING** *bitman(BIT_STRING, $val, $maxbits)*
returns a string representation of $val, with a maximum of $maxbits characters. 

*bits.ph7* script to test linrary. 