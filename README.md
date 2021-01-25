# Sifreleyici
OTP (Vernam cipher) File Encryptor and Decryptor. Skips first 512 bytes of the file to make header stay intact. Then uses XOR cipher encryption algorithm to encrypt 1 MB of the file with random OTP key and generates random key file. Decryption is done with same method in reverse.

Parameters:<br />
Sifreleyici --sifrele [file_path] -> Encrypts file with random bits.<br />
Sifreleyici --sifrecoz [file_path] --anahtar [key_file_path] --> Decrypts encrypted file with specific key file.<br />
Sifreleyici --yardim --> Help menu.<br />
Sifreleyici --versiyon --> Shows version.<br />
