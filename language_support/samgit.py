# AT THE MOMENT THIS JUST HAS TO BE IMPORTED AS import samgit
# EXPECT INSTALLATION OPTIONS FOR CLEANER CODE SOON, TILL THEN JUST PASTE THIS INTO YOUR DIRECTORY I SUPPOSE
import subprocess
import inspect

def PUSHY(file_path, message):
    frame = inspect.currentframe().f_back
    line = frame.f_lineno
    called_from = frame.f_code.co_filename
    result = subprocess.run(
        ["samgit", "pushy", file_path, message, str(line), called_from],
        capture_output=True, text=True
    )
    return result.returncode == 0