#include <Arduino.h>

#define MAX_SIZE 16
// c character to encrypt, i lowest key value, j highest key value
uint8_t charEncrypt(int c, int i, int j){

  uint8_t osec = j;
  uint8_t sec = osec<<4;

  uint8_t one_res = (sec | i);
  uint8_t one_enc = one_res + c;

  return one_enc;
}

uint8_t charDecrypt(int c, int i, int j) {

  uint8_t osec = j;
  uint8_t sec = osec<<4;

  uint8_t one_res = (sec | i);
  uint8_t one_enc = 0;
  if (c > one_res){
    one_enc = c - one_res;
  } else {
    one_enc = one_res - c;
  }

  return one_enc;
}

uint8_t * encrypt(char text[], uint8_t key[], uint8_t text_size, uint8_t key_size){

  static uint8_t *buff;
  buff = malloc(MAX_SIZE * sizeof(uint8_t));
  uint8_t init = 0;
  uint8_t key_pair [2];

  for (uint8_t i = 0; i < text_size - 1; i++){

    if(init > (key_size + 1)){        // 0 < 6
      init = 0;                            // 0 -> reset init
    }

    uint8_t c = (int) text[i];
    key_pair[0] = key[init];
    key_pair[1] = key[init + 1];
    init++;
    uint8_t res = charEncrypt(c, key_pair[0], key_pair[1]);
    buff[i + 1] = res;
  }

  return buff;
}

uint8_t * decrypt(uint8_t encrypted[], uint8_t key[], uint8_t text_size, uint8_t key_size){

  static uint8_t *buff;
  buff = malloc(MAX_SIZE * sizeof(uint8_t));
  uint8_t init = 0;
  uint8_t key_pair [2];

  for (uint8_t i = 0; i < text_size - 1; i++){

    if(init > (key_size + 1)){        // 0 < 6
      init = 0;                            // 0 -> reset init
    }

    key_pair[0] = key[init];
    key_pair[1] = key[init + 1];
    init++;
    uint8_t res = charDecrypt(encrypted[i + 1], key_pair[0], key_pair[1]);
    //Serial.print(res);
    //Serial.println();
    buff[i] = res;
  }

  return buff;
}

void setup() {
  Serial.begin(115200);

  char text[] = "HELLO WORLD";
  uint8_t key[] = {0x5, 0x2, 0x2, 0x8, 0x4, 0x6, 0x5};
  uint8_t *buff_encrypt;
  uint8_t *buff_decrypt;

  buff_encrypt = encrypt(text, key, sizeof(text),sizeof(key));
  Serial.println("Encrypting...");
  for (uint8_t i = 0; i < sizeof(text) -1; i++){
    Serial.print(*(buff_encrypt + i));
    Serial.print(",");
  }

  Serial.println();

  buff_decrypt = decrypt(buff_encrypt, key, sizeof(text), sizeof(key));
  Serial.println("Decrypting...");
  for (uint8_t j = 0; j < sizeof(text) - 1; j++){
    Serial.print(*(buff_decrypt + j));
    Serial.print(",");
  }

  Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:
}