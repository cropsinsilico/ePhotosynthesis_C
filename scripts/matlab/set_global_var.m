function set_global_var(var, value)
    arguments
        var (1,1) string
        value (1,1) double
    end
    eval("global " + var + ";");
    eval(var + " = value;");
end
