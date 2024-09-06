The files contained in this directory define the default constants for the various modules. The files are used to generate enum files using utils/generate_enum.py during the build (invoked by cmake). The prefix indicates the module and the suffix indicates the type of constants. Not all modules use every type of constants.

-----------------------------------
Prefix	  	Description
-----------------------------------
BF		Electron transfer from reduced platquinone until the generation of ATP and NADPH including io transfer through thylakoid membrane and ATP synthesis process
FI		Light energy absorption, transfer, primary charge separation, electro transfer around PSII
PR		Photorespiration
PS		Calvin cycle, starch synthesis, triose phosphate export
RROEA		Redox regulation of enzyme activities?
RedoxReg	Redox regulation of enzyme activities (EPS + RuAct + RA)
RuACT		Rubisco activation
SUCS		Sucrose? (langmm: This is just a guess)
XanCycle	Xanthophylls cycle

-----------------------------------
Suffix	  	Description
-----------------------------------
KE		Equilibrium constants
RC	  	Rate constants
COND	  	Initial conditions
POOL	  	Pool constants
