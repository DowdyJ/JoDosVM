<h1>LC3 Virtual Machine and Assembler</h1>
This is my implementation of a VM for the LC3. The VM will only run on Unix-like machines but the assembler should run on either.

To build, run <code>make</code> in the base directory.
<br>
To run the VM, run <code>./lc3.pls [path] [swap_endianness]</code><br>
<ul>
<li><code>path</code>:             relative or abolute path to input binary code using forward slashes."</li>
<li><code>swap_endianness</code>:  whether to swap byte order during loading of binary image. Acceptable values are TRUE or FALSE. Should typically be TRUE.</li>
</ul>
<br>
To run the Assembler, run <code>./assembler.pls [path] [swap_endianness]</code><br>
<ul>
<li><code>path</code>:             relative or abolute path to input LC3 assembly code file using forward slashes."</li>
<li><code>swap_endianness</code>:  whether to swap byte order during writing of binary image. Acceptable values are TRUE or FALSE. Should typically be TRUE.</li>
</ul>

<h2>LC3 VM</h2>
The VM portion of the project could not have been made without the help of <a href="https://www.jmeiners.com/lc3-vm/">this excellent site</a>. 


<h2>LC3 Assembler</h2>
I made the assembler portion of this project by reading the project docs and reverse engineering pre-assembled binaries and comparing them to their assembly code. For the two projects I tested against it produces byte identical binary. 

As per the LC3 standard, SEMICOLON marks a comment. Comments can be on a line by themselves or after an instruction.

Tokens can be seperated by any combination of TAB, SPACE or COMMA. 

<h3>Labels</h3>
Labels may be placed on the same line as an instruction or alone on the line above it. 
Labels are not case sensitive. LOOP is regarded as the same as Loop/lOOp/loop and so on.

<h3>Assembler Macros</h3>
The following macros are valid for use with the assembler:

<h4>.ORIG</h4>
Requires an argument in hex format. (e.g. x3000)

<h4>.FILL</h4>
.FILL will accept an argument of either a literal value in either hex format (x123), LC3 decimal (#123) or decimal literals (123). 
.FILL also accepts an argument of a label. In this case, this will resolve to the position in memory of the label. (e.g. 0x3000 + label line index) 

<h4>.STRINGZ</h4>
.STRINGZ takes an argument enclosed by double quotes. Semi-colons inside of STRINGZ arguments are not culled as comments. The escape characters ESC \e) and NEWLINE (\n) are replaced with their actual ASCII values. Other escape codes unsupported.

<h4>.END</h4>
.END marks the end of the file. All text is stripped past the .END before any interpretation occurs.
