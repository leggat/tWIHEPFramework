# tWIHEPFramework
Documentation can be found in Framework_documentation_slides.pdf

Please refer to the slides for reference and ask if anything is unclear!


Old documentation:

tW code based on IHEP framework

The newTest.sh bash script is enough to run the code on a single file of the tW MC as it stands.

Arguments in newTest.sh are as follows:
	  -config		 - Generic configuration file containing links to object defintions, cuts and dataset weights.
	  -inlist   		 - List of files to run over. In newTest.sh this is a single tW file (for testing purposes)
	  -hfile    		 - The file that stores the generated histograms. Note in newTest.sh this puts out to the directory hists which does not exist by default.
	  -skimfile 		 - The output skim file after all cuts are applied. Note in newTest.sh this puts out to the directory skims which does not exist by default.
	  -mc	      	  	 - Tells the script that it's simulation. Leave off for data.
`	  -UseTotalEvtFromFile 	 - This gets the number of MC events from a file (already saved in MCDatasets.info)
	  -MCatNLO		 - Tells the code to use MCatNLO weights.
	  -PileUpWgt		 - Read the pileup weight from the event tree
	  -BWgt			 - Read the b-weight fro the event tree

TODO:
	 - Is there a way to make one plot class plots every step? Ask Huaqiao.
