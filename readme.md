# STARREACHER — Proiect OOP C++ cu SDL2

## Descriere

Acest proiect este un joc de tip space invaders, dezvoltat în C++ folosind SDL2 pentru grafică și sunet. Codul folosește paradigma programării orientate pe obiect și pune accent pe organizarea modulară, moștenire și polimorfism.

---

## Structura principală a proiectului

- `src/Entity.h`, `Entity.cpp`: Clasă de bază abstractă pentru entitățile din joc (jucător, inamic, etc.).
- `src/Player.h`, `Player.cpp`: Clasa pentru jucător, derivată din Entity.
- `src/Enemy.h`, `Enemy.cpp`: Clasa pentru inamic, derivată din Entity.
- `src/Texture.h`, `Texture.cpp`: Managementul texturilor jocului.
- `src/Ecran.h`, `Ecran.cpp`: Gestionarea ecranului și randării.

---

## Cerințe OOP implementate în proiect

### Tema 1: Programare orientată pe obiect

- **Clase cu câmpuri și metode private/public**  
  Toate clasele majore (`Player`, `Enemy`, `Entity`, etc.) folosesc atribute private/protected și metode publice, respectând principiile de încapsulare.

- **Constructori și destructori**  
  Fiecare clasă importantă are constructor(i) expliciți. Destructori sunt prezenți implicit; nu sunt destructori expliciți implementați.

- **Moștenire și ierarhii**  
  `Player` și `Enemy` moștenesc din clasa de bază `Entity`.

- **Funcții virtuale și polimorfism**  
  `Entity` definește funcții virtuale precum `update()` și `render()`, care sunt suprascrise (`override`) în clasele derivate. Acest lucru permite tratarea polimorfă a entităților în cod (ex: vector de pointeri la Entity).

- **Specificați de acces (`private`, `protected`, `public`)**  
  Sunt folosiți corect la nivel de clasă, metode și atribute.

- **Folosirea STL**  
  Sunt folosite containere STL precum `std::vector` (ex: pentru liste de entități sau proiectile), ceea ce elimină riscul de memory leaks.

---

### Tema 2: Moștenire și polimorfism

- **Moștenire între clase**  
  Se evidențiază relația "is-a" între `Entity` și clasele derivate (`Player`, `Enemy`).

- **Polimorfism cu funcții virtuale**  
  Se folosește polimorfismul pentru a apela funcții specifice fiecărei entități prin pointeri sau referințe la `Entity`.

---

### Tema 3: Organizare modulară și extindere

- **Structură modulară a codului**  
  Fiecare clasă importantă este definită în fișiere separate `.h` și `.cpp`, facilitând extinderea și mentenanța proiectului.

- **Utilizarea STL pentru gestiunea entităților**  
  Folosirea vectorilor de entități permite extinderea rapidă cu alte tipuri de obiecte în joc.

---

## Cerințe parțial implementate:

- Supraincărcare operatori `<<` și `>>` pentru clase
- Funcții sau clase template (șablon)
- Utilizare explicită de `dynamic_cast` sau `static_cast`

---

## Cum se compilează și rulează

1. Se folosește CMake pentru generarea fișierelor de build.
2. Se compilează proiectul folosind un compilator C++ compatibil.
3. Se rulează executabilul rezultat (asigurați-vă că fișierele SDL2 necesare sunt prezente în directorul executabilului).

---

## Observații finale

Proiectul STARREACHER demonstrează o bază solidă de programare OOP în C++, folosind moștenire, polimorfism și organizare modulară, fiind ușor de extins pentru adăugarea de noi funcționalități sau pentru implementarea altor concepte avansate de OOP.

