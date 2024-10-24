// Master Line Follower
// m1 kiri depan, m2 kanan depan, m3 kiri belakang, kanan belakang

// Nano sebagai Master
#include <EEPROM.h>

// Function SLAVE
void slave(char address, int data1) {
  Serial.println(address);
  Serial.println(data1);
  delay(15);
}

void slave(char address, int data1, int data2) {
  Serial.println(address);
  Serial.println(data1);
  Serial.println(data2);
  delay(15);
}

void slave(char address, int data1, int data2, int data3) {
  Serial.println(address);
  Serial.println(data1);
  Serial.println(data2);
  Serial.println(data3);
  delay(15);
}


// Slave Address
char motorKananDepan = 'A';     // Kanan depan
char motorKiriDepan = 'B';      // Kiri depan
char motorKananBelakang = 'C';  // Kanan Belakang
char motorKiriBelakang = 'D';   // Kiri Belakang

// Konfigurasi LineFollower
#define button2 12
#define button1 5
#define pinCom 4

int sensor[8] = { A7, A6, A5, A4, A3, A2, A1, A0 };                     // Pin Sensor
int sensorMin[8] = { 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023 };  // Nilai Min awal
int sensorMax[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };                          // Nilai Maks awal
int sensorMinAkhir[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };                     // Nilai Min akhir
int sensorMaxAkhir[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };                     // Nilai Maks akhir
int averageSensor[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };                      // Nilai rata-rata
int riilSensor[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };                         // Nilai riil sensor
int outputSensor[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int calibrateButton = 1;
int threshold = 0;
int batasAtas[8] = { 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023 };
int count = 0;
bool run = true;
bool run1 = true;
bool run2 = true;
bool kalib = true;
int errorLine = 0;
int button1Clicked = 0;
int button2Clicked = 0;
int cp = 0;
int spd = 60;

void setup() {
  Serial.begin(115200);
  pinMode(pinCom, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  digitalWrite(pinCom, HIGH);

  // Baca Data dari EEPROM
  for (int i = 0; i < 8; i++) {
    batasAtas[i] = EEPROM.read(i);
  }
}


void loop() {
  checkButton1();
  checkButton2();

  // Calibration
  if (button1Clicked == 1 && button2Clicked == 1 && kalib == true) {
    kalibrasi();
    kalib = false;
  }

  // Reset button
  if (button1Clicked == 3) {
    run1 = true;
    run2 = true;
    // run1trig = true;
    // run2trig = true;
    kalib = true;
    button1Clicked = 0;
    button2Clicked = 0;
  }

  if (button1Clicked == 2 && run1 == true) {

    delay(1000);

    // Jalan mengikuti garis sampai perempatan merah
    int waktu28 = millis();
    int waktu17;
    while (true) {
      waktu17 = millis();
      if (waktu17 - waktu28 >= 5000) {
        break;
      }
      if (spd < 80) {
        spd += 2;
      }
      errorLine = detectLine();
      lineFol(errorLine, spd);
      // Serial.println("maju SEBENTAR!!!");
    }

    while (true) {
      errorLine = detectLine();
      if (outputSensor[7] == 1) {
        break;
      }
      lineFol(errorLine, 40);
    }


    // Maju 1 detik
    int waktu0 = millis();
    int waktu1;
    while (true) {
      waktu1 = millis();
      if (waktu1 - waktu0 >= 1000) {
        break;
      }
      diagonal_kiri(70);
      // Serial.println("maju SEBENTAR!!!");
    }

    while (true) {
      errorLine = detectLine();
      if (outputSensor[3] == 1 || outputSensor[4] == 1 || outputSensor[0] == 1) {
        break;
      }
      diagonal_kiri(70);
    }

    // Maju 0.5 detik
    int waktu12 = millis();
    int waktu123;
    while (true) {
      waktu123 = millis();
      if (waktu123 - waktu12 >= 300) {
        break;
      }
      driveMotor(35, 35);
      // Serial.println("maju SEBENTAR!!!");
    }

    // rotasi kanan ketika perempatan
    while (true) {
      rotasi_kiri(50);
      // Serial.println("rotasi");
      errorLine = detectLine();
      if (errorLine >= -3 && errorLine <= 3) {
        break;
      }
    }

    // Maju 2 detik
    int waktu5 = millis();
    int waktu6;
    spd = 50;
    while (true) {
      waktu6 = millis();
      if (waktu6 - waktu5 >= 4000) {
        break;
      }
      errorLine = detectLine();
      lineFol(errorLine, spd);
      // Serial.println("maju SEBENTAR!!!");
    }

    // Lurus Teruss
    while (true) {
      errorLine = detectLine();
      if (outputSensor[7] == 1) {
        break;
      }
      // Serial.println("Lurus TEeruss");
      lineFol(errorLine, 40);
    }

    // Maju 0.5 detik
    int waktu63 = millis();
    int waktu74;
    while (true) {
      waktu74 = millis();
      if (waktu74 - waktu63 >= 400) {
        break;
      }
      driveMotor(35, 35);
      // Serial.println("maju SEBENTAR!!!");
    }

    while (true) {
      rotasi_kanan(60);
      // Serial.println("rotasi");
      errorLine = detectLine();
      if (errorLine >= -3 && errorLine <= 3) {
        break;
      }
    }

    // Maju 2 detik
    int waktu45 = millis();
    int waktu61;
    spd = 70;
    while (true) {
      waktu61 = millis();
      if (waktu61 - waktu45 >= 4500) {
        break;
      }
      // if (spd < 80) {
      //   spd += 2;
      // }
      errorLine = detectLine();
      lineFol(errorLine, spd);
      // Serial.println("maju SEBENTAR!!!");
    }

    int waktu98 = millis();
    int waktu213;
    while (true) {
      waktu213 = millis();
      if (waktu213 - waktu98 >= 800) {
        break;
      }
      rotasi_kiri(80);
      // Serial.println("maju SEBENTAR!!!");
    }
    stop();
    delay(500);
    mode(2);
    run1 = false;
  }



  // Jalanin dari Retry Zone
  if (button2Clicked == 2 && run2 == true) {

    // Horizontal Kanan sampai garis lurus
    while (true) {
      horizontal_kiri(50);
      errorLine = detectLine();
      if (outputSensor[3] == 1 || outputSensor[4] == 1 || outputSensor[0] == 1) {
        break;
      }
    }

    // Maju 1 detik
    int waktu0 = millis();
    int waktu1;
    while (true) {
      waktu1 = millis();
      if (waktu1 - waktu0 >= 1000) {
        break;
      }
      diagonal_kiri(70);
      // Serial.println("maju SEBENTAR!!!");
    }

    while (true) {
      errorLine = detectLine();
      if (outputSensor[3] == 1 || outputSensor[4] == 1 || outputSensor[0] == 1) {
        break;
      }
      diagonal_kiri(70);
    }

    // Maju 0.5 detik
    int waktu12 = millis();
    int waktu123;
    while (true) {
      waktu123 = millis();
      if (waktu123 - waktu12 >= 300) {
        break;
      }
      driveMotor(35, 35);
      // Serial.println("maju SEBENTAR!!!");
    }

    // rotasi kanan ketika perempatan
    while (true) {
      rotasi_kiri(50);
      // Serial.println("rotasi");
      errorLine = detectLine();
      if (errorLine >= -3 && errorLine <= 3) {
        break;
      }
    }

    // Maju 2 detik
    int waktu5 = millis();
    int waktu6;
    spd = 50;
    while (true) {
      waktu6 = millis();
      if (waktu6 - waktu5 >= 4000) {
        break;
      }
      errorLine = detectLine();
      lineFol(errorLine, spd);
      // Serial.println("maju SEBENTAR!!!");
    }

    // Lurus Teruss
    while (true) {
      errorLine = detectLine();
      if (outputSensor[7] == 1) {
        break;
      }
      // Serial.println("Lurus TEeruss");
      lineFol(errorLine, 40);
    }

    // Maju 0.5 detik
    int waktu63 = millis();
    int waktu74;
    while (true) {
      waktu74 = millis();
      if (waktu74 - waktu63 >= 400) {
        break;
      }
      driveMotor(35, 35);
      // Serial.println("maju SEBENTAR!!!");
    }

    while (true) {
      rotasi_kanan(60);
      // Serial.println("rotasi");
      errorLine = detectLine();
      if (errorLine >= -3 && errorLine <= 3) {
        break;
      }
    }

    // Maju 2 detik
    int waktu45 = millis();
    int waktu61;
    spd = 70;
    while (true) {
      waktu61 = millis();
      if (waktu61 - waktu45 >= 4500) {
        break;
      }
      // if (spd < 80) {
      //   spd += 2;
      // }
      errorLine = detectLine();
      lineFol(errorLine, spd);
      // Serial.println("maju SEBENTAR!!!");
    }

    int waktu98 = millis();
    int waktu213;
    while (true) {
      waktu213 = millis();
      if (waktu213 - waktu98 >= 800) {
        break;
      }
      rotasi_kiri(80);
      // Serial.println("maju SEBENTAR!!!");
    }
    stop();
    delay(500);
    mode(2);
    run2 = false;
  }
  stop();
}

int lastError;
void lineFol(int err, int baseSpeed) {
  if (err == -4) {
    driveMotor(baseSpeed + 20, baseSpeed - 50);
  } else if (err == -3) {
    driveMotor(baseSpeed, baseSpeed - 20);
  } else if (err == -2) {
    driveMotor(baseSpeed, baseSpeed - 10);
  } else if (err == -1) {
    driveMotor(baseSpeed, baseSpeed - 5);
  } else if (err == 0) {
    driveMotor(baseSpeed, baseSpeed);
  } else if (err == 1) {
    driveMotor(baseSpeed - 5, baseSpeed);
  } else if (err == 2) {
    driveMotor(baseSpeed - 10, baseSpeed);
  } else if (err == 3) {
    driveMotor(baseSpeed - 20, baseSpeed);
  } else if (err == 4) {
    driveMotor(baseSpeed - 50, baseSpeed + 20);
  } else {
    if (lastError == -4) {
      driveMotor(baseSpeed + 20, baseSpeed - 150);
    } else if (lastError == -3) {
      driveMotor(baseSpeed, baseSpeed - 100);
    } else if (lastError == -2) {
      driveMotor(baseSpeed, baseSpeed - 50);
    } else if (lastError == -1) {
      driveMotor(baseSpeed, baseSpeed - 5);
    } else if (lastError == 0) {
      driveMotor(baseSpeed, baseSpeed);
    } else if (lastError == 1) {
      driveMotor(baseSpeed - 5, baseSpeed);
    } else if (lastError == 2) {
      driveMotor(baseSpeed - 50, baseSpeed);
    } else if (lastError == 3) {
      driveMotor(baseSpeed - 100, baseSpeed);
    } else if (lastError == 4) {
      driveMotor(baseSpeed - 150, baseSpeed + 20);
    }
  }

  lastError = err;
}

void driveMotor(int kanan, int kiri) {
  slave(motorKananDepan, -kanan);
  slave(motorKiriDepan, -kiri);
  slave(motorKananBelakang, -kanan);
  slave(motorKiriBelakang, -kiri);
};

void driveMotor(int kananDepan, int kiriDepan, int kananBelakang, int kiriBelakang) {
  slave(motorKananDepan, -kananDepan);
  slave(motorKiriDepan, -kiriDepan);
  slave(motorKananBelakang, -kananBelakang);
  slave(motorKiriBelakang, -kiriBelakang);
};


void cekControlSpeed() {
  driveMotor(100, 100, 100, 100);
  // Serial.println("Maju");
  delay(2000);
  driveMotor(0, 100, 0, 100);
  // Serial.println("Kanan");
  delay(2000);
  driveMotor(100, 0, 100, 0);
  // Serial.println("Kiri");
  delay(2000);
  driveMotor(-100, -100, -100, -100);
  // Serial.println("Mundur");
  delay(2000);
}


// Gerak Robot
void belok_kiri(int spd) {
  driveMotor(spd, (spd * (0.4)), spd, (spd * (0.4)));
};

void belok_kanan(int spd) {
  driveMotor((spd * (0.4)), spd, (spd * (0.4)), spd);
};

void rotasi_kiri(int spd) {
  driveMotor(spd, -spd, spd, -spd);
};

void rotasi_kanan(int spd) {
  driveMotor(-spd, spd, -spd, spd);
};


void diagonal_kiri(int spd) {
  driveMotor(spd, 0, 0, spd);
};

void diagonal_kanan(int spd) {
  driveMotor(0, spd, spd, 0);
};

void horizontal_kiri(int spd) {
  driveMotor(spd, -spd, -spd, spd + 20);
};

void horizontal_kanan(int spd) {
  driveMotor(-spd, spd, spd, -spd);
};

void stop() {
  driveMotor(0, 0, 0, 0);
};

void kalibrasi() {
  int sensorValue[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  // Kirim integer 30 sebagai angka kalibrasi ke uno
  // Serial.println(30);

  // Tanda Mulai Kalibrasi
  Serial.println("Mulai Kalibrasi!");

  // loop untuk mencari nilai maks dan min dari sensor selama 10 detik
  int counter = -1;
  unsigned long waktuMulaiKalibrasi = millis();
  while (millis() - waktuMulaiKalibrasi < 8000) {
    counter++;
    if (counter < 15) {
      horizontal_kanan(30);
    } else if (counter > 15 && counter < 45) {
      horizontal_kiri(30);
    } else if (counter > 45 && counter < 75) {
      horizontal_kanan(30);
    } else if (counter > 75 && counter < 105) {
      horizontal_kiri(30);
    } else {
      horizontal_kanan(30);
    }

    // Serial.println("Baca Sensor");
    for (int i = 0; i < 8; i++) {
      sensorValue[i] = analogRead(sensor[i]);

      // Cari Nilai Maks
      if (sensorValue[i] > sensorMax[i]) {
        sensorMax[i] = sensorValue[i];
      }

      // Cari Nilai Min
      if (sensorValue[i] < sensorMin[i]) {
        sensorMin[i] = sensorValue[i];
      }
    }
    // Serial.println(counter);
  }

  // Matiin Motor
  driveMotor(0, 0);

  // Masukkan Nilai Sensor Max dan Min ke Nilai Akhir
  for (int i = 0; i < 8; i++) {
    sensorMaxAkhir[i] = sensorMax[i];
    sensorMinAkhir[i] = sensorMin[i];
  }

  // Hitung rata-rata sensor
  for (int i = 0; i < 8; i++) {
    averageSensor[i] = (sensorMax[i] + sensorMin[i]) / 2;
  }

  // Tampilkan nilai rata-rata sensor
  Serial.println("Nilai Tengah : ");
  for (int i = 0; i < 8; i++) {
    Serial.print("Sensor ke - ");
    Serial.print(i + 1);
    Serial.print("\t");
    Serial.println(averageSensor[i]);
  }

  // Nilai Max
  Serial.println("Max : ");
  for (int i = 0; i < 8; i++) {
    if (i == 7) {
      Serial.println(sensorMax[i]);
    } else {
      Serial.print(sensorMax[i]);
      Serial.print("\t");
    }
  }

  // Nilai Min
  Serial.println("Min : ");
  for (int i = 0; i < 8; i++) {
    if (i == 7) {
      Serial.println(sensorMin[i]);
    } else {
      Serial.print(sensorMin[i]);
      Serial.print("\t");
    }
  }

  // Penentuan Threshold
  for (int i = 0; i < 8; i++) {
    threshold = ((sensorMaxAkhir[i] - averageSensor[i]) / 4);
    batasAtas[i] = averageSensor[i] + (threshold);
    EEPROM.write(i, batasAtas[i]);
    // batasBawah = averageSensor[i];
    Serial.print("Batas Atas ke - ");
    Serial.print(i);
    Serial.print("\t : ");
    Serial.println(batasAtas[i]);
    // Serial.print("Batas Bawah ke - ");
    // Serial.print(i);
    // Serial.print("\t : ");
    // Serial.println(batasBawah);
  }

  // // Bersihkan nilai maks dan min sensor untuk kalibrasi berikutnya
  // for (int i = 0; i < 8; i++) {
  //   sensorMax[i] = 0;
  //   sensorMin[i] = 1023;
  // }
}

void tampilkanOutput() {
  for (int i = 0; i < 8; i++) {
    if (i < 7) {
      Serial.print(outputSensor[i]);
      Serial.print("\t");
    } else {
      Serial.println(outputSensor[i]);
    }
  }
}

void perbandinganData() {
  Serial.print("Average");
  Serial.print("\t \t : ");
  for (int i = 0; i < 8; i++) {
    if (i < 7) {
      Serial.print(averageSensor[i]);
      Serial.print("\t");
    } else {
      Serial.println(averageSensor[i]);
    }
  }

  Serial.print("Batas");
  Serial.print("\t \t : ");
  for (int i = 0; i < 8; i++) {
    if (i < 7) {
      Serial.print(batasAtas[i]);
      Serial.print("\t");
    } else {
      Serial.println(batasAtas[i]);
    }
  }

  Serial.print("Riil Sensor");
  Serial.print("\t : ");
  for (int i = 0; i < 8; i++) {
    if (i < 7) {
      Serial.print(riilSensor[i]);
      Serial.print("\t");
    } else {
      Serial.println(riilSensor[i]);
    }
  }
}

int detectLine() {
  // Menentukan nilai dari line

  // Penentuan 0 dan 1 (1 Untuk Putih dan 0 Selain Putih)
  for (int i = 0; i < 8; i++) {
    if (analogRead(sensor[i]) > batasAtas[i]) {
      outputSensor[i] = 1;
    } else {
      outputSensor[i] = 0;
    }
  }

  if ((outputSensor[0] == 0) && (outputSensor[1] == 0) && (outputSensor[2] == 0) && (outputSensor[3] == 0) && (outputSensor[4] == 0) && (outputSensor[5] == 1) && (outputSensor[6] == 1) && (outputSensor[7]) == 0) {  //  0 0 0 0 0 1 1 0  (Ke Kanan)
    return 4;
  } else if ((outputSensor[0] == 0) && (outputSensor[1] == 0) && (outputSensor[2] == 0) && (outputSensor[3] == 0) && (outputSensor[4] == 1) && (outputSensor[5] == 1) && (outputSensor[6] == 1) && (outputSensor[7]) == 0) {  //  0 0 0 0 1 1 1 0  (Ke Kanan)
    return 3;
  } else if ((outputSensor[0] == 0) && (outputSensor[1] == 0) && (outputSensor[2] == 0) && (outputSensor[3] == 0) && (outputSensor[4] == 1) && (outputSensor[5] == 1) && (outputSensor[6] == 0) && (outputSensor[7] == 0)) {  // 0 0 0 0 1 1 0 0   (Ke Kanan Dikit)
    return 2;
  } else if ((outputSensor[0] == 0) && (outputSensor[1] == 0) && (outputSensor[2] == 0) && (outputSensor[3] == 1) && (outputSensor[4] == 1) && (outputSensor[5] == 1) && (outputSensor[6] == 0) && (outputSensor[7] == 0)) {  //   0 0 0 1 1 1 0 0  (Ke Kanan Dikit Bgt)
    return 1;
  } else if ((outputSensor[0] == 0) && (outputSensor[1] == 0) && (outputSensor[2] == 0) && (outputSensor[3] == 1) && (outputSensor[4] == 1) && (outputSensor[5] == 0) && (outputSensor[6] == 0) && (outputSensor[7] == 0)) {  // 0 0 0 1 1 0 0 0  (Lurus aja)
    return 0;
  } else if ((outputSensor[0] == 0) && (outputSensor[1] == 0) && (outputSensor[2] == 1) && (outputSensor[3] == 1) && (outputSensor[4] == 1) && (outputSensor[5] == 0) && (outputSensor[6] == 0) && (outputSensor[7] == 0)) {  // 0 0 1 1 1 0 0 0   (Ke Kiri Dikit Bgt)
    return -1;
  } else if ((outputSensor[0] == 0) && (outputSensor[1] == 0) && (outputSensor[2] == 1) && (outputSensor[3] == 1) && (outputSensor[4] == 0) && (outputSensor[5] == 0) && (outputSensor[6] == 0) && (outputSensor[7] == 0)) {  //  0 0 1 1 0 0 0 0  (Ke Kiri Dikit)
    return -2;
  } else if ((outputSensor[0] == 0) && (outputSensor[1] == 1) && (outputSensor[2] == 1) && (outputSensor[3] == 1) && (outputSensor[4] == 0) && (outputSensor[5] == 0) && (outputSensor[6] == 0) && (outputSensor[7] == 0)) {  //  0 1 1 1 0 0 0 0 (Ke Kiri)
    return -3;
  } else if ((outputSensor[0] == 0) && (outputSensor[1] == 1) && (outputSensor[2] == 1) && (outputSensor[3] == 0) && (outputSensor[4] == 0) && (outputSensor[5] == 0) && (outputSensor[6] == 0) && (outputSensor[7] == 0)) {  //  0 1 1 0 0 0 0 0 (Ke Kiri Bgt)
    return -4;
  } else {
    return 50;
  }
}

void checkButton1() {
  int readButton = digitalRead(button1);
  while (readButton == 0) {
    readButton = digitalRead(button1);
    if (readButton == 1) {
      button1Clicked += 1;
      delay(10);
    }
  }

  Serial.print("Button1 : ");
  Serial.println(button1Clicked);
}

void checkButton2() {
  int readButton = digitalRead(button2);
  while (readButton == 0) {
    readButton = digitalRead(button2);
    if (readButton == 1) {
      button2Clicked += 1;
      delay(10);
    }
  }

  Serial.print("Read : ");
  Serial.println(readButton);
}

int prevReadLine;
void checkPoint(int readLine) {
  if (readLine == 16 && prevReadLine != 16) {
    cp += 1;
  }
  // Serial.print("CP : ");
  // Serial.println(cp);
  prevReadLine = readLine;
}

void mode(int a) {
  if (a == 1) {
    digitalWrite(pinCom, HIGH);
  } else {
    int i = 0;
    while (i < 10) {
      digitalWrite(pinCom, HIGH);
      Serial.print("I");
      Serial.println(10);
      delay(15);
      i++;
    }

    digitalWrite(pinCom, LOW);
  }
}
