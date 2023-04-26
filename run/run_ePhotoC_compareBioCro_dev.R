leafT = 25
PAR = 1500
Cis = seq(10,1000,by=50)
input_fn = "InputEvn2.txt"
Assim_ephoto = array(0,c(1,length(Cis)))
for (i in 1:length(Cis))
{
   con <- file(input_fn, "r")
   inputs = readLines(con)
   inputs[1] = paste("CO2\t",Cis[i],sep="")
   inputs[2] = paste("PAR\t",PAR,sep="")
   close(con)
   writeLines(inputs,input_fn)
   
#this ePhotosynthesis exe may not be consistent with my EPS model
   ephoto_cmd = paste("./ePhoto -e ",input_fn," -a InputATPCost.txt -d 4 -T ",leafT," -n Einput7.txt -m 2500",sep="")
   print(ephoto_cmd)
   system(ephoto_cmd)
   Assim_ephoto[i] = read.delim("output.data",header = FALSE) 
   Sys.sleep(1) 
}
X = cbind(Cis,PAR,Assim_ephoto)
write.table(X,"ACi_ephoto_CPP.csv",col.names = FALSE,row.names = FALSE)
