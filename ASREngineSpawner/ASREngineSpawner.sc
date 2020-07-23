Import('env')




env.Append(CPPPATH=['.'])



lib_tgt = env.Library(['ASREngineSpawner.cpp', 'ProcessManager.cpp', 'ASREngineWidget.ui'])
env.Append(LIBS=lib_tgt)
prg = env.Program('ASREngineSpawner.exe', ['main.cpp'])
for i in env.Glob('*'):
    print(str(i))
Default(prg)
