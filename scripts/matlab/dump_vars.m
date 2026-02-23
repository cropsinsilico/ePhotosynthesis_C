function fname = dump_vars(fname, vars)
    disp("Writing variables to file: " + fname);
    
    fileID = fopen(fname,'w');

    varStr = vardict2string(vars);
    fprintf(fileID, '%s', varStr);

end
