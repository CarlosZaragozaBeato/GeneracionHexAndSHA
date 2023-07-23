import os
import hashlib

def create_folders_for_sha256():
    base_path = "./sha/"
    
    if not os.path.exists(base_path):
        os.makedirs(base_path)

    for i in range(256):
        initial_hex = format(i, '02x')  
        folder_path = os.path.join(base_path, initial_hex)

        if not os.path.exists(folder_path):
            os.makedirs(folder_path)

if __name__ == "__main__":
    create_folders_for_sha256()
