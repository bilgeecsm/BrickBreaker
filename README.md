# BrickBreaker
 Tuğla kırma oyunu Arduino projesi

Bu proje, mikrodenetleyici tabanlı bir oyun
makinesi oluşturmak için çeşitli araçları kullanmayı
amaçlamaktadır.

![image](https://github.com/user-attachments/assets/875256e4-af09-4884-a95e-e48d1d07d892)

Fig.1: Sistem


• Ana Menü ve Seçenekler: Oyun başlangıcında,
kullanıcıya "Başlat" ve "Çıkış" seçenekleri
sunulur. Kullanıcı, yukarı ve aşağı tuşlarıyla
seçenekler arasında gezinebilir ve "Başlat"
seçeneğini onaylayarak oyuna başlayabilir.

![image](https://github.com/user-attachments/assets/ec2c003a-3534-4b9a-ae84-2150a66a6069)

Fig.2: Menü


• Oyun Başlangıcı: "Başlat" seçeneği seçildiğinde,
oyun ekranı açılır ve oyun başlar. Oyuncu, potansiyometre yardımı ile paleti
sağa sola hareket ettirerek topu oyun alanında
tutmaya çalışır.

![image](https://github.com/user-attachments/assets/0528764f-b458-48a5-85d2-279e81a49aea)
Fig.3: Oyun


• Tuğlaların Kırılması ve Skor: Top, paletle
çarpışarak tuğlaları kırar. Her kırılan tuğla,
oyuncunun skorunu bir puan artırır. Bu skor, 7
segment displayde gösterilir.
• Can Sistemi ve LED Göstergesi: Oyuncunun
başlangıçta 3 canı vardır. Her top paletin altına
düştüğünde, oyuncunun bir canı azalır. Canlar,
LED'lerle gösterilir ve her can kaybıyla bir LED
söner.
• Özel Objeler ve Can Kazanımı: %10 olasılıkla
düşen özel objeler, oyuncuya bir can kazandırır. Bu
objeler, kırılan tuğlalardan aşağı doğru hareket eder
ve alındığında oyuncunun can sayısı artar. Can
kazanımıyla birlikte LED göstergesi güncellenir.
• Oyun Sonu ve Skor Gösterimi: Oyuncunun 3 canı
bittiğinde, ekranda son skor görüntülenir ve oyuncu
ana menüye geri döner.

![image](https://github.com/user-attachments/assets/8a209a75-49f7-48ac-8fb4-67e29baa9975)

Fig.4: Skor, can ledleri, potansiyometre, seven segment display ve butonlar


• Renk Değişimi: Arka plan ve tuğlaların rengi, ışık
sensörüne bağlı olarak değişir. Işık açıkken arka
plan siyah, tuğlalar ise beyaz olur.
• Hız Artışı ve Bölüm Geçişi: Her bölüm
geçildiğinde, topun hızı %20 artar. Bu şekilde oyun
zorluk seviyesi artar ve oyuncunun becerisi sınanır.
