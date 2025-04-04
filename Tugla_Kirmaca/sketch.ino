#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// 7 Segment Display için Gerekenler
#define a 17
#define b 16
#define c 2
#define d 3
#define e 4
#define f 5
#define g 6

#define digit1 14
#define digit2 15

char Num[10] = {
	0b1111110, // 0
  0b0110000, // 1
  0b1101101, // 2
  0b1111001, // 3
  0b0110011, // 4
  0b1011011, // 5
  0b1011111, // 6
  0b1110000, // 7
  0b1111111, // 8
  0b1111011  // 9  
};

// Hoparlör Ayarları
#define SPEAKER_PIN 18
#define NOTE_C4 262
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_E6 1319
#define NOTE_DS5 622
#define NOTE_D5  587

// Ekran Ayarları
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

int renkOyun = WHITE;
int lightMode = 0;

// Ledler
#define LED_HP1 8
#define LED_HP2 9
#define LED_HP3 10

// Butonlar
int seciliTus = 0;
#define BUTON_PIN1 13 // Onaylama butonu - space
#define BUTON_PIN2 12 // Yukarı Butonu - arrowup
#define BUTON_PIN3 11 // Aşağı Butonu - arrowdown
#define BUTON_PIN4 7 // Renk Değişme Butonu - arrowright


// Palet Ayarları
#define PADDLE_WIDTH 35
#define PADDLE_HEIGHT 3
#define PADDLE_SPEED 5

// Top Ayarları
#define BALL_SIZE 4
#define BALL_SPEED_INCREMENT_PERCENT 20

// Tuğla Ayarları
#define BRICK_WIDTH 20
#define BRICK_HEIGHT 6
#define BRICK_GAP 2

// Obje Ayarları
#define OBJE_SIZE 4

// Topun Başlayacağı yer
#define BALL_START_X (SCREEN_WIDTH / 2 - BALL_SIZE / 2)
#define BALL_START_Y (SCREEN_HEIGHT - PADDLE_HEIGHT - BALL_SIZE - 1)

#define SCORE_X 100
#define SCORE_Y 0

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int paddleX;
int ballX, ballY;
float ballSpeedX, ballSpeedY;
const float ballDefaultSpeedX = 1.0f;
const float ballDefaultSpeedY = -1.0f;

// Skor değişkeni
int score=0;
int yCarpma = 0;
int xCarpma = 0;
volatile unsigned long bekleme;

// Toplam bolum sayısı
int bolumSayisi = 7;

// Oyun durum değişkeni
int anaMenu = 1;
int cikisEkran = 0;
int oyunBitme = 0;  // 1 ise oyun bitmiştir, 0 ise oyun devam ediyordur
int canSayisi = 3;

// Oyunun hangi bölümde olduğunu tutan değişken
int bolumIndex = 1;

// Her bolume ait tuğla dizaynı
// x koordinatları, y koordinatları, kırılma durumu (1 kırılmamış, 0 kırılmış)
// şeklinde saklanır

// 1. Bölüm
int bolum1[18][3] = {
  {0, 0, 1}, {1, 0, 1}, {2, 0, 1}, {3, 0, 1}, {4, 0, 1}, {5, 0, 1},
  {0, 1, 1}, {1, 1, 1}, {2, 1, 1}, {3, 1, 1}, {4, 1, 1}, {5, 1, 1}, 
  {0, 2, 1}, {1, 2, 1}, {2, 2, 1}, {3, 2, 1}, {4, 2, 1}, {5, 2, 1}
};

// 2. Bölüm
int bolum2[18][3] = {
  {0, 0, 1}, {1, 0, 0}, {2, 0, 1}, {3, 0, 1}, {4, 0, 0}, {5, 0, 1},
  {0, 1, 0}, {1, 1, 1}, {2, 1, 0}, {3, 1, 0}, {4, 1, 1}, {5, 1, 0}, 
  {0, 2, 1}, {1, 2, 0}, {2, 2, 1}, {3, 2, 1}, {4, 2, 0}, {5, 2, 1}
};


// 3. Bölüm
int bolum3[18][3] = {
  {0, 0, 1}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0}, {5, 0, 1},
  {0, 1, 0}, {1, 1, 1}, {2, 1, 1}, {3, 1, 1}, {4, 1, 1}, {5, 1, 0}, 
  {0, 2, 0}, {1, 2, 0}, {2, 2, 1}, {3, 2, 1}, {4, 2, 0}, {5, 2, 0}
};


// 4. Bölüm
int bolum4[18][3] = {
  {0, 0, 1}, {1, 0, 1}, {2, 0, 0}, {3, 0, 0}, {4, 0, 1}, {5, 0, 1},
  {0, 1, 0}, {1, 1, 1}, {2, 1, 1}, {3, 1, 1}, {4, 1, 1}, {5, 1, 0}, 
  {0, 2, 1}, {1, 2, 1}, {2, 2, 0}, {3, 2, 0}, {4, 2, 1}, {5, 2, 1}
};

// 5. Bölüm
int bolum5[18][3] = {
  {0, 0, 1}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0}, {5, 0, 1},
  {0, 1, 0}, {1, 1, 1}, {2, 1, 1}, {3, 1, 1}, {4, 1, 1}, {5, 1, 0}, 
  {0, 2, 0}, {1, 2, 1}, {2, 2, 0}, {3, 2, 0}, {4, 2, 1}, {5, 2, 0}
};

// 6. Bölüm
int bolum6[18][3] = {
  {0, 0, 1}, {1, 0, 1}, {2, 0, 1}, {3, 0, 1}, {4, 0, 1}, {5, 0, 1},
  {0, 1, 1}, {1, 1, 0}, {2, 1, 0}, {3, 1, 0}, {4, 1, 0}, {5, 1, 1}, 
  {0, 2, 1}, {1, 2, 0}, {2, 2, 0}, {3, 2, 0}, {4, 2, 0}, {5, 2, 1}
};

// 7. Bölüm
int bolum7[18][3] = {
  {0, 0, 1}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 1}, {5, 0, 0},
  {0, 1, 0}, {1, 1, 1}, {2, 1, 0}, {3, 1, 1}, {4, 1, 0}, {5, 1, 1}, 
  {0, 2, 0}, {1, 2, 0}, {2, 2, 1}, {3, 2, 0}, {4, 2, 0}, {5, 2, 0}
};

// Düşen objeleri tutacak matris
int objeler[18][3] = {
  {0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0}, {5, 0, 0},
  {0, 1, 0}, {1, 1, 0}, {2, 1, 0}, {3, 1, 0}, {4, 1, 0}, {5, 1, 0}, 
  {0, 2, 0}, {1, 2, 0}, {2, 2, 0}, {3, 2, 0}, {4, 2, 0}, {5, 2, 0}
};


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  Serial.begin(9600);
  
  setupLED(); // Ledler
  setupBUTTON(); // Butonlar
  setupSSEGMENT(); // 7-Segmentler
  pinMode(SPEAKER_PIN, OUTPUT);
  oyunuHazirla();

  randomSeed(analogRead(0));
}

// SETUP Fonksiyonları
void setupLED() {
  pinMode(LED_HP1, OUTPUT);
  pinMode(LED_HP2, OUTPUT);
  pinMode(LED_HP3, OUTPUT);
}

void setupBUTTON() {
  pinMode(BUTON_PIN1, INPUT_PULLUP);
  pinMode(BUTON_PIN2, INPUT_PULLUP);
  pinMode(BUTON_PIN3, INPUT_PULLUP);
  pinMode(BUTON_PIN4, INPUT_PULLUP);
}

void setupSSEGMENT() {
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  
  digitalWrite(digit1, 0); // kapat digit1
  digitalWrite(digit2, 1); // kapat digit2
}



void loop() {
  segmentAyarla(); // 7 Segment kontrol
  renkDegis(); // Oyun rengi kontrol
  
  if (cikisEkran == 1) { // Çıkış Ekranı
    drawCikisEkran();
  }
  else {
    if (anaMenu == 1) { // Menü Ekranı
      drawMenu();
      butonKontrol();
    }
    else { // Oyun Ekranı
      if (oyunBitme == 0) { // Oyun oynanmaya devam ediyorsa yapılacaklar
        movePaddle();
        moveBall();
        moveObjeler();
        checkLevelCompletion();
      }
      drawGame();
    }
  }

}

// Ana Menüyü çizmeye yarayan fonksiyon
void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(renkOyun);
  display.setCursor(SCREEN_WIDTH / 15, SCREEN_HEIGHT / 4);
  display.println("BRICK BREAKER GAME\n");
  if (seciliTus == 0) {
    display.println("     ->BASLAT");
    display.print("       CIKIS");
  }
  else {
    display.println("       BASLAT");
    display.print("     ->CIKIS");
  }
  
  display.display();
}


// Oyunu Çizmeye yarayan fonksiyon
void drawGame() {
  display.clearDisplay();
  
    // Tuğlalarla çarpışmayı kontrol eden kısım
  if (bolumIndex >= 1 && bolumIndex <= bolumSayisi && oyunBitme == 0) {
    int (*bolum)[18][3];
    switch (bolumIndex) {
      case 1: bolum = &bolum1; break;
      case 2: bolum = &bolum2; break;
      case 3: bolum = &bolum3; break;
      case 4: bolum = &bolum4; break;
      case 5: bolum = &bolum5; break;
      case 6: bolum = &bolum6; break;
      case 7: bolum = &bolum7; break;
    }
    for (int index = 0; index < 18; index++) {
      if ((*bolum)[index][2] == 1) {
        int brickX = (*bolum)[index][0] * (BRICK_WIDTH + BRICK_GAP);
        int brickY = (*bolum)[index][1] * (BRICK_HEIGHT + BRICK_GAP);
        display.fillRect(brickX, brickY, BRICK_WIDTH, BRICK_HEIGHT, renkOyun);
      } 
    }
  }
  switch (canSayisi) {
    case 0:
      digitalWrite(LED_HP1, LOW);
      digitalWrite(LED_HP2, LOW);
      digitalWrite(LED_HP3, LOW);
      break;
    case 1:
      digitalWrite(LED_HP1, HIGH);
      digitalWrite(LED_HP2, LOW);
      digitalWrite(LED_HP3, LOW);
      break;
    case 2:
      digitalWrite(LED_HP1, HIGH);
      digitalWrite(LED_HP2, HIGH);
      digitalWrite(LED_HP3, LOW);
      break;
    case 3:
      digitalWrite(LED_HP1, HIGH);
      digitalWrite(LED_HP2, HIGH);
      digitalWrite(LED_HP3, HIGH);
      break;
    default:
      break;
  }
    if (oyunBitme == 1) {
    display.setTextSize(2);
    display.setTextColor(renkOyun);
    display.setCursor(SCREEN_WIDTH / 15, SCREEN_HEIGHT / 3);
    display.println("GAME OVER");
    display.print(" SKOR:");
    display.print(score);
    display.display();


    // OYUN BİTTİKTEN SONRA OYUNU SIFIRLAYIP BAŞLANGIÇ EKRANINA DÖNME
    for(bekleme = 0; bekleme <= 2500000; bekleme++)
    {}
    oyunuHazirla();
    bolumleriSifirla();
    anaMenu = 1;

    
    return; 
  }
  

  
  // palet çizimi
  display.fillRect(paddleX, SCREEN_HEIGHT - PADDLE_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT, renkOyun);

  // top çizimi
  display.fillCircle(ballX, ballY, BALL_SIZE, renkOyun);

  // objelerin çizimi
  drawObjeler();

  display.display();
}

// Paleti hareket ettirme fonksiyonu
void movePaddle() {
  int potValue = analogRead(A0);
  int paddleSpeed = map(potValue, 0, 1023, -PADDLE_SPEED, PADDLE_SPEED);
  paddleX += paddleSpeed;

  if (paddleX < 0) {
    paddleX = 0;
  } else if (paddleX > SCREEN_WIDTH - PADDLE_WIDTH) {
    paddleX = SCREEN_WIDTH - PADDLE_WIDTH;
  }

}

// Topu Hareket ettirme fonksiyonu
void moveBall() {
  
  ballX += ballSpeedX;
  ballY += ballSpeedY;

  // duvarla çarpışmayı kontrol etme
  if (ballX <= 0 || ballX >= SCREEN_WIDTH - BALL_SIZE) {
    ballSpeedX = -ballSpeedX;
  }
  if (ballY <= 0) {
    ballSpeedY = -ballSpeedY;
  }

  // paletle çarpışmayı kontrol etme
  if (ballY >= SCREEN_HEIGHT - PADDLE_HEIGHT - BALL_SIZE && ballX + BALL_SIZE >= paddleX && ballX <= paddleX + PADDLE_WIDTH) {
    ballSpeedY = -ballSpeedY;
    vurmaEfektiPalet();
  }

  // Tuğlalarla çarpışmayı kontrol eden kısım
  if (bolumIndex >= 1 && bolumIndex <= bolumSayisi) {
    int (*bolum)[18][3];
    switch (bolumIndex) {
      case 1: bolum = &bolum1; break;
      case 2: bolum = &bolum2; break;
      case 3: bolum = &bolum3; break;
      case 4: bolum = &bolum4; break;
      case 5: bolum = &bolum5; break;
      case 6: bolum = &bolum6; break;
      case 7: bolum = &bolum7; break;
    }

    // 2 tuğlaya aynı yönden aynı anda çarpınca yönü 2 kez değişmesin diye
    if (yCarpma == 1) {
      yCarpma = 2;
    }
    else if (yCarpma == 2) {
      yCarpma = 0;
    }

    if (xCarpma == 1) {
      xCarpma = 2;
    }
    else if (xCarpma == 2) {
      xCarpma = 0;
    }

    for (int index = 0; index < 18; index++) {
      if ((*bolum)[index][2] == 1) {
        int brickX = (*bolum)[index][0] * (BRICK_WIDTH + BRICK_GAP);
        int brickY = (*bolum)[index][1] * (BRICK_HEIGHT + BRICK_GAP);
  
        // Tuğla ile topun her bir yüzeyi arasındaki çarpışmayı kontrol et
        if (ballX + BALL_SIZE >= brickX && ballX <= brickX + BRICK_WIDTH && ballY + BALL_SIZE >= brickY && ballY <= brickY + BRICK_HEIGHT) {
          // Çarpışma yönünü belirle
          int collisionX = ballX + BALL_SIZE / 2;
          int collisionY = ballY + BALL_SIZE / 2;

          // Üst ve alt yönler
          if (collisionY < brickY || collisionY > brickY + BRICK_HEIGHT) {
            if (!yCarpma) {
              ballSpeedY = -ballSpeedY;
              yCarpma = 1;
            }
            
          }
          // Sol ve sağ yönler
          else if (collisionX < brickX || collisionX > brickX + BRICK_WIDTH) {
            if (!xCarpma) {
              ballSpeedX = -ballSpeedX;
              xCarpma = 1;
            }
            
          }
                    

          // Tuğlayı kır
          (*bolum)[index][2] = 0;
          vurmaEfektiTugla();
          objeDusurme(index);
          score++;

        }
      } 
    }
  }

  // Topun raketin altına düşüp düşmediğini kontrol eden kısım
  if (ballY >= SCREEN_HEIGHT) {
    gameOver();
  }
  
}

// Objeleri Çizer
void drawObjeler() {
  for(int index = 0; index < 18; index++) {
    if (objeler[index][2] == 1) {
      int objeX = objeler[index][0] * (BRICK_WIDTH + BRICK_GAP) + OBJE_SIZE * 2;
      int objeY = index * (BRICK_HEIGHT + BRICK_GAP) / 6 + objeler[index][1];
      // 8x8 piksel boyutunda kalp şekli
      uint8_t heart[] = {
        B01000100,
        B11101110,
        B11111110,
        B11111110,
        B11111110,
        B01111100,
        B00111000,
        B00010000
      };

      // Kalp şeklini çizme
      for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
          if (bitRead(heart[y], x)) {
            display.drawPixel(objeX + x, objeY + y, WHITE);
          }
        }
      }
    }
  }
}

// Tuğlaya vurulduğunda obje düşürmeyi sağlama fonksiyonu
void objeDusurme(int index) {
  int olasilik = random(1, 11); // 5 ise obje düşer , 1/10 ihtimal
  if (olasilik == 5) {
    objeler[index][2] = 1;
  }
}

// Düşen objelerin hareketini sağlar
void moveObjeler() {
  for(int index = 0; index < 18; index++) {
    if (objeler[index][2] == 1) {
      int objeY = objeler[index][1];
      int objeX = objeler[index][0] * (BRICK_WIDTH + BRICK_GAP) + OBJE_SIZE * 2;
      objeler[index][1] = objeY + 1;
      objeY = objeY + OBJE_SIZE * 4 + 1;

      // Paletle çarpışmayı kontrol etme
      if (objeY >= SCREEN_HEIGHT - PADDLE_HEIGHT - OBJE_SIZE && objeX + OBJE_SIZE >= paddleX && objeX - 3 <= paddleX + PADDLE_WIDTH) {
        objeler[index][2] = 0;
        canAlmaEfekti();
        canSayisi++;
        if (canSayisi > 3) {
          canSayisi = 3;
        }
      }
      // Çarpışma kontrol etme
      if (objeY >= SCREEN_HEIGHT) // Yere çarparsa
      {
        objeler[index][2] = 0;
      }
    }
  }
}


void gameOver() {
  canSayisi--;
  if (canSayisi == 0) {
    oyunBitme = 1;
    ballSpeedY = 0;
    ballSpeedX = 0;
    kaybetmeEfekti();
  }
  else {
    canKaybetmeEfekti();
    paddleX = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
    ballX = BALL_START_X;
    ballY = BALL_START_Y;
  }
  
}

// Bölüm Geçişi sağlanacak mı kontrol eden kısım
void checkLevelCompletion() {
  int (*bolum)[18][3];

  // Hangi bölümde olduğumuzu kontrol edelim
  switch (bolumIndex) {
    case 1: bolum = &bolum1; break;
    case 2: bolum = &bolum2; break;
    case 3: bolum = &bolum3; break;
    case 4: bolum = &bolum4; break;
    case 5: bolum = &bolum5; break;
    case 6: bolum = &bolum6; break;
    case 7: bolum = &bolum7; break;
    default: return; // Geçersiz bölüm numarası, çık
  }

  // Tüm tuğlaların kırılıp kırılmadığını kontrol et
  for (int index = 0; index < 18; index++) {
    if ((*bolum)[index][2] == 1) {
      // En az bir tuğla hala kırılmadıysa, bu bölüm tamamlanmamış demektir
      return;
    }
  }

  // Eğer buraya kadar geldiysek, bölüm tamamlandı demektir
  // Bir sonraki bölüme geç
  bolumIndex++;

  // Eğer tüm bölümler tamamlanmışsa, oyunu bitir
  if (bolumIndex > bolumSayisi) {
    oyunBitme = 1;


    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(renkOyun);
    display.setCursor(SCREEN_WIDTH / 15, SCREEN_HEIGHT / 3);
    display.println("BUTUN BOLUMLER");
    display.println("      BITTI\n");
    display.print("      SKOR:");
    display.print(score);
    display.display();
    kazanmaEfekti();

    // OYUN BİTTİKTEN SONRA OYUNU SIFIRLAYIP BAŞLANGIÇ EKRANINA DÖNME
    for(bekleme = 0; bekleme <= 2500000; bekleme++)
    {}
    oyunuHazirla();
    bolumleriSifirla();
    anaMenu = 1;

  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(renkOyun);
    display.setCursor(SCREEN_WIDTH / 15, SCREEN_HEIGHT / 3);
    display.println("   BOLUM BITTI");
    display.println(" SONRAKINE GECILIYOR\n");
    display.print("   GUNCEL SKOR:");
    display.print(score);
    display.display();


    // OYUN BİTTİKTEN SONRA OYUNU SIFIRLAYIP BAŞLANGIÇ EKRANINA DÖNME
    for(bekleme = 0; bekleme <= 2500000; bekleme++)
    {}

    // Yeni bölümün başına dön
    paddleX = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
    ballX = BALL_START_X;
    ballY = BALL_START_Y;
    ballSpeedX = ballSpeedX * 1.2f;
    ballSpeedY = ballSpeedY * 1.2f;
    
  }
}


void oyunuHazirla() {
  paddleX = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
  ballX = BALL_START_X;
  ballY = BALL_START_Y;
  ballSpeedX = ballDefaultSpeedX;
  ballSpeedY = ballDefaultSpeedY;
  score = 0;
  bolumIndex = 1;
  oyunBitme = 0;
  canSayisi = 3;

}

void bolumleriSifirla() {
  // 1. Bölüm
  int original_bolum1[18][3] = {
    {0, 0, 1}, {1, 0, 1}, {2, 0, 1}, {3, 0, 1}, {4, 0, 1}, {5, 0, 1},
    {0, 1, 1}, {1, 1, 1}, {2, 1, 1}, {3, 1, 1}, {4, 1, 1}, {5, 1, 1}, 
    {0, 2, 1}, {1, 2, 1}, {2, 2, 1}, {3, 2, 1}, {4, 2, 1}, {5, 2, 1}
  };

  // 2. Bölüm
  int original_bolum2[18][3] = {
    {0, 0, 1}, {1, 0, 0}, {2, 0, 1}, {3, 0, 1}, {4, 0, 0}, {5, 0, 1},
    {0, 1, 0}, {1, 1, 1}, {2, 1, 0}, {3, 1, 0}, {4, 1, 1}, {5, 1, 0}, 
    {0, 2, 1}, {1, 2, 0}, {2, 2, 1}, {3, 2, 1}, {4, 2, 0}, {5, 2, 1}
  };


  // 3. Bölüm
  int original_bolum3[18][3] = {
    {0, 0, 1}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0}, {5, 0, 1},
    {0, 1, 0}, {1, 1, 1}, {2, 1, 1}, {3, 1, 1}, {4, 1, 1}, {5, 1, 0}, 
    {0, 2, 0}, {1, 2, 0}, {2, 2, 1}, {3, 2, 1}, {4, 2, 0}, {5, 2, 0}
  };


  // 4. Bölüm
  int original_bolum4[18][3] = {
    {0, 0, 1}, {1, 0, 1}, {2, 0, 0}, {3, 0, 0}, {4, 0, 1}, {5, 0, 1},
    {0, 1, 0}, {1, 1, 1}, {2, 1, 1}, {3, 1, 1}, {4, 1, 1}, {5, 1, 0}, 
    {0, 2, 1}, {1, 2, 1}, {2, 2, 0}, {3, 2, 0}, {4, 2, 1}, {5, 2, 1}
  };

  // 5. Bölüm
  int original_bolum5[18][3] = {
    {0, 0, 1}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0}, {5, 0, 1},
    {0, 1, 0}, {1, 1, 1}, {2, 1, 1}, {3, 1, 1}, {4, 1, 1}, {5, 1, 0}, 
    {0, 2, 0}, {1, 2, 1}, {2, 2, 0}, {3, 2, 0}, {4, 2, 1}, {5, 2, 0}
  };

  // 6. Bölüm
  int original_bolum6[18][3] = {
    {0, 0, 1}, {1, 0, 1}, {2, 0, 1}, {3, 0, 1}, {4, 0, 1}, {5, 0, 1},
    {0, 1, 1}, {1, 1, 0}, {2, 1, 0}, {3, 1, 0}, {4, 1, 0}, {5, 1, 1}, 
    {0, 2, 1}, {1, 2, 0}, {2, 2, 0}, {3, 2, 0}, {4, 2, 0}, {5, 2, 1}
  };

  // 7. Bölüm
  int original_bolum7[18][3] = {
    {0, 0, 1}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 1}, {5, 0, 0},
    {0, 1, 0}, {1, 1, 1}, {2, 1, 0}, {3, 1, 1}, {4, 1, 0}, {5, 1, 1}, 
    {0, 2, 0}, {1, 2, 0}, {2, 2, 1}, {3, 2, 0}, {4, 2, 0}, {5, 2, 0}
  };

  // Düşen objeleri tutacak matris
int original_objeler[18][3] = {
  {0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {4, 0, 0}, {5, 0, 0},
  {0, 1, 0}, {1, 1, 0}, {2, 1, 0}, {3, 1, 0}, {4, 1, 0}, {5, 1, 0}, 
  {0, 2, 0}, {1, 2, 0}, {2, 2, 0}, {3, 2, 0}, {4, 2, 0}, {5, 2, 0}
};

  memcpy(bolum1, original_bolum1, sizeof(original_bolum1));
  memcpy(bolum2, original_bolum2, sizeof(original_bolum2));
  memcpy(bolum3, original_bolum3, sizeof(original_bolum3));
  memcpy(bolum4, original_bolum4, sizeof(original_bolum4));
  memcpy(bolum5, original_bolum5, sizeof(original_bolum5));
  memcpy(bolum6, original_bolum6, sizeof(original_bolum6));
  memcpy(bolum7, original_bolum7, sizeof(original_bolum7));
  memcpy(objeler, original_objeler, sizeof(original_objeler));
}

// Ana Menüde kullanılan butonları kontrol eden fonksiyon
void butonKontrol() {
  // Oyun Başlatma Butonu
  if (digitalRead(BUTON_PIN1) == LOW) {
    // Oyun Başlatılacak
    if (seciliTus == 0) {
      anaMenu = 0;
    }
    // Çıkış Yapılacak
    else {
      cikisEkran = 1;
    }
  }

  if (digitalRead(BUTON_PIN2) == LOW) {
    seciliTus = 0;
  }

  if (digitalRead(BUTON_PIN3) == LOW) {
    seciliTus = 1;
  }

}

// Çıkış Ekranı çizdirme
void drawCikisEkran() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(renkOyun);
  display.setCursor(SCREEN_WIDTH / 15, SCREEN_HEIGHT / 5);
  display.println("     OYUNUMUZA");
  display.println("    GOSTERDIGINIZ");
  display.println("      ILGI ICIN");
  display.println("     TESEKKURLER");
      
  display.display();
}

// Oyun rengi değiştirme fonksiyonu
void renkDegis() {
  if (digitalRead(BUTON_PIN4) == LOW) {
    if (lightMode == 0) {
      display.invertDisplay(true);
      lightMode = 1;
      for(bekleme = 0; bekleme <= 100000; bekleme++)
      {}
    }
    else {
      lightMode = 0;
      display.invertDisplay(false);
      for(bekleme = 0; bekleme <= 100000; bekleme++)
      {}
    }
  }
}

// 7 Segment Fonksiyonları
// 1- Sayı Yazdırma
void number(int n)
{
 digitalWrite(a, n & 0b1000000);
 digitalWrite(b, n & 0b0100000);
 digitalWrite(c, n & 0b0010000);
 digitalWrite(d, n & 0b0001000);
 digitalWrite(e, n & 0b0000100);
 digitalWrite(f, n & 0b0000010);
 digitalWrite(g, n & 0b0000001);
}

// 2- Nasıl Yazdıracağını belirleme
void segmentAyarla() {
  int d1 = score / 10;
  int d2 = score % 10;
    
  digitalWrite(digit2, 1); // Off digit2
  digitalWrite(digit1, 1);
  number(Num[d1]); 
  digitalWrite(digit1, 0); 
    
  digitalWrite(digit2, 1); // Off digit2
  digitalWrite(digit1, 1); // Off digit1
    
  number(Num[d2]); 
  digitalWrite(digit2, 0);
}

// Tuğlaya vurma efekti
void vurmaEfektiTugla() {
  tone(SPEAKER_PIN, NOTE_C4);
  for(bekleme = 0; bekleme <= 10000; bekleme++) {}
  noTone(SPEAKER_PIN);
}

// Palete vurma efekti
void vurmaEfektiPalet() {
  tone(SPEAKER_PIN, NOTE_C5);
  for(bekleme = 0; bekleme <= 10000; bekleme++) {}
  noTone(SPEAKER_PIN);
}

// Can toplama efekti
void canAlmaEfekti() {
  tone(SPEAKER_PIN, NOTE_E6);
  for(bekleme = 0; bekleme <= 10000; bekleme++) {}
  noTone(SPEAKER_PIN);
}

// Oyunu kazanma efekti
void kazanmaEfekti() {
  int melody[] = {
    262, 196,196, 220, 196,0, 247, 262};

  int noteDurations[] = {
    4, 8, 8, 4,4,4,4,4 };

  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    tone(SPEAKER_PIN, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(SPEAKER_PIN);
  }
}

// Oyunu Kaybetme efekti
void kaybetmeEfekti() {
  // Oyun Kaybetme Sesi
  tone(SPEAKER_PIN, NOTE_DS5);
  for(bekleme = 0; bekleme <= 100000; bekleme++) {}
  tone(SPEAKER_PIN, NOTE_D5);
  for(bekleme = 0; bekleme <= 100000; bekleme++) {}
  tone(SPEAKER_PIN, NOTE_CS5);
  for(bekleme = 0; bekleme <= 100000; bekleme++) {}
  for (byte i = 0; i < 10; i++) {
    for (int pitch = -20; pitch <= 0; pitch++) {
      tone(SPEAKER_PIN, NOTE_C5 + pitch);
      for(bekleme = 0; bekleme <= 1600; bekleme++) {}
    }
  }
  noTone(SPEAKER_PIN);
}

// Can kaybetme efekti
void canKaybetmeEfekti() {
  tone(SPEAKER_PIN, NOTE_CS5 - 50);
  for(bekleme = 0; bekleme <= 25000; bekleme++) {}
  tone(SPEAKER_PIN, NOTE_C5 - 60);
  for(bekleme = 0; bekleme <= 25000; bekleme++) {}
  noTone(SPEAKER_PIN);
}
