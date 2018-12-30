

symbols = symbols = [
    ("==", "EQ_EQ"),
    ("=", "EQ"),
    (">>=", "GT_GT_EQ"),
    (">>", "GT_GT"),
    (">=", "GT_EQ"),
    (">", "GT"),
    ("<<=", "LE_LE_EQ"),
    ("<<", "LE_LE"),
    ("<=", "LE_EQ"),
    ("<", "LE"),
    ("!=", "NOT_EQ"),
    ("!", "NOT"),
    ("--", "DEC"),
    ("-=", "SUB_EQ"),
    ("-", "SUB"),
    ("++", "INC"),
    ("+=", "ADD_EQ"),
    ("+", "ADD"),
    ("*=", "MUL_EQ"),
    ("*", "MUL"),
    ("/=", "DIV_EQ"),
    ("/", "DIV"),
    ("%=", "MOD_EQ"),
    ("%", "MOD"),
    ("&&", "AND"),
    ("&=", "AND_EQ"),
    ("&", "BIT_AND"),
    ("||", "OR"),
    ("|=", "OR_EQ"),
    ("|", "BIT_OR"),
    ("^=", "XOR_EQ"),
    ("^", "BIT_XOR"),
    ("(", "PAREN_L"),
    (")", "PAREN_R"),
    ("{", "BRACE_L"),
    ("}", "BRACE_R"),
    ("[", "BRACKET_L"),
    ("]", "BRACKET_R"),
    (".", "DOT"),
    (",", "COM"),
    (";", "SEM"),
]

words = [
    'async',
    'await',
    'break',
    'case',
    'catch',
    'class',
    'const',
    'continue',
    'default',
    'delete',
    'do',
    'else',
    'export',
    'extends',
    'false',
    'for',
    'function',
    'if',
    'import',
    'in',
    'instanceof',
    'let',
    'new',
    'null',
    'return',
    'static',
    'super',
    'switch',
    'this',
    'throw',
    'true',
    'try',
    'var',
    'while',
]

words.sort()

comp = ["EOF", "id", "string", "int", "float"]

for i in comp:
    print '    T(%s, "<%s>", 0) \\' % (i.upper(), i)

for i in symbols:
    print '    T(%s, "%s", %d) \\' % (i[1], i[0], len(i[0]))

for i in words:
    print '    T(%s, "%s", %d) \\' % (i.upper(), i, len(i))
