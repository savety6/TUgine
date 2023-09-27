import subprocess
import os

def RunConan(build_type):
    subprocess.run((
        'conan', 'install', '.',
        '--build', 'missing',
        '--output-folder=./dependencies'
        f'--settings=build_type={build_type}'
    ))

def RunPremake(action):
    subprocess.run((
        'premake5', action,
    ))

def RunPremakeOsIndependent():
    # check if the platform is linux
    if os.name == 'posix':
        RunPremake('gmake2')
        return
    # check if the platform is windows
    if os.name == 'nt':
        RunPremake('vs2022')
        return
    # check if the platform is mac
    if os.name == 'mac':
        RunPremake('xcode4')
    

if __name__ == '__main__':
    RunConan('Debug')
    RunConan('Release')
    RunPremakeOsIndependent()
