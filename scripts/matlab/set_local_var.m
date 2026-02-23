function set_local_var(var, value)
    arguments
        var (1,1) string
        value (1,1) double
    end
    evalin('caller', var + " = value;");
end
