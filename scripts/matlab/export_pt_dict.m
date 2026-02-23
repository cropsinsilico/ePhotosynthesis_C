function out = export_pt_dict(mod, pt, x)
    out = x;
    eval("global " + mod + "_" + pt + "_DICT;");
    eval(mod + "_" + pt + "_DICT = out;");
end
