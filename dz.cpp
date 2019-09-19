#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "windows.h"

using namespace std;

#pragma region "Skup.h"

class Skup
{

protected:
	virtual void pisi(ostream& o) const = 0;

public:
	virtual ~Skup() {}

	virtual bool PostojanjeElementa(int k) const = 0;
	virtual Skup& UmetniElement(int k) = 0;
	Skup& operator+=(int k);

	virtual Skup& ObrisiElement(int k) = 0;
	Skup& operator-=(int k);
	virtual Skup& ObrisiOpsegVrednosti(int min, int max) = 0;

	virtual int BrojElemenata() const = 0;

	virtual void IsprazniSkup() = 0;

	enum Greska { INDEKS, TIP };

	virtual Skup* Razlika(const Skup& s2) const = 0;
	friend Skup* operator-(const Skup& s1, const Skup& s2);

	friend ostream& operator<<(ostream& o, const Skup& s);

};

#pragma endregion

#pragma region "PerformanceCalculator.h"

class PerformanceCalculator
{
	LARGE_INTEGER startCounter;
	LARGE_INTEGER stopCounter;
	LARGE_INTEGER frequency;

public:
	PerformanceCalculator()
	{
		startCounter.QuadPart = stopCounter.QuadPart = 0;
		QueryPerformanceFrequency(&frequency);
	}

	void start()
	{
		QueryPerformanceCounter(&startCounter);
	}

	void stop()
	{
		QueryPerformanceCounter(&stopCounter);
	}

	double elapsedMillis() const
	{
		return (stopCounter.QuadPart - startCounter.QuadPart)*1000. / frequency.QuadPart;
	}

};

#pragma endregion

#pragma region "AnalizatorPerformansi.h"

class AnalizatorPerformansi
{

public:
	static double FormirajSkup(Skup& skup, int velicina, int seme);
	static double Umetanje(Skup& skup, int novi);
	static double Brisanje(Skup& skup, int k);
	static double Pretraga(Skup& skup, int k, bool& postoji);
	static double BrisanjeOpsegaVrednosti(Skup& skup, int min, int max);
	static double Razlika(Skup & s1, Skup & s2, int velicina);

};

#pragma endregion

#pragma region "BSTSkup.h"

class BSTSkup : public Skup
{

private:
	class Cvor
	{
	public:
		int kljuc;
		Cvor* levi;
		Cvor* desni;

		Cvor(int _kljuc, Cvor* _levi = nullptr, Cvor* _desni = nullptr) :kljuc(_kljuc), levi(_levi), desni(_desni) {}
	};

	Cvor* koren;
	int n;

	void kopiraj(const BSTSkup& s);
	void premesti(BSTSkup& ss);
	void obrisi();

	Cvor* pronadiElement(int k, Cvor* koren) const;

	void pisi(ostream& o) const override;

public:
	BSTSkup() : koren(nullptr), n(0) {}
	BSTSkup(const BSTSkup& s) { kopiraj(s); }
	BSTSkup(BSTSkup&& ss) { premesti(ss); }
	~BSTSkup() override { obrisi(); }

	BSTSkup& operator=(const BSTSkup& s);
	BSTSkup& operator=(BSTSkup&& s);

	bool PostojanjeElementa(int k) const override;
	BSTSkup& UmetniElement(int k) override;

	BSTSkup& ObrisiElement(int k) override;
	BSTSkup& ObrisiOpsegVrednosti(int min, int max) override;

	int BrojElemenata() const override { return n; }

	void IsprazniSkup() override;

	BSTSkup* Razlika(const Skup& s2) const;
};

#pragma endregion

#pragma region "DCLLSkup.h"

class DCLLSkup : public Skup
{

private:
	class Element
	{
	public:
		Element * prethodni;
		int broj;
		Element* sledeci;

		Element(int _broj, Element* _prethodni = nullptr, Element* _sledeci = nullptr) :broj(_broj), prethodni(_prethodni), sledeci(_sledeci) {}
	};

	class Zaglavlje : public Element
	{
	public:
		// polje broj iz klase Element se koristi za cuvanje broja elemenata liste

		Zaglavlje(int _broj, Element* _prethodni = nullptr, Element* _sledeci = nullptr) : Element(_broj, _prethodni, _sledeci) {}
	};

	enum Operacija { DODAVANJE, BRISANJE };

	Zaglavlje* pZaglavlje;

	void kopiraj(const DCLLSkup& s);
	void premesti(DCLLSkup& s);
	void obrisi();

	inline void inicijalizuj();

	inline void azurirajZaglavlje(Operacija op);

	inline void dodajNakon(Element *p, int k);
	inline void dodajPre(Element *p, int k);

	void pisi(ostream& o) const override;

public:
	DCLLSkup() { inicijalizuj(); }
	DCLLSkup(const DCLLSkup& s) { kopiraj(s); }
	DCLLSkup(DCLLSkup&& ss) { premesti(ss); }
	~DCLLSkup() override { obrisi(); }

	DCLLSkup& operator=(const DCLLSkup& s);
	DCLLSkup& operator=(DCLLSkup&& s);

	bool PostojanjeElementa(int k) const override;
	DCLLSkup& UmetniElement(int k) override;

	DCLLSkup& ObrisiElement(int k) override;
	DCLLSkup& ObrisiOpsegVrednosti(int min, int max) override;

	int BrojElemenata() const override;

	void IsprazniSkup() override;

	DCLLSkup* Razlika(const Skup& s2) const;

};

#pragma endregion

#pragma region "Stek.h"

template <class T>
class Stek
{

private:
	struct Element
	{
		T sadrzaj;
		Element* sledeci;

		Element(T _sadrzaj, Element* _sledeci = nullptr) :sadrzaj(_sadrzaj), sledeci(_sledeci) {}
	};

	Element* pokazivac;

	void kopiraj(const Stek& s);
	void premesti(Stek&& s);
	void obrisi();

public:
	Stek() :pokazivac(nullptr) {}
	Stek(const Stek& s) { kopiraj(s); }
	Stek(Stek&& s) { premesti(s); }
	~Stek() { obrisi(); }

	Stek& operator=(const Stek& s);
	Stek& operator=(Stek&& s);

	void Dodaj(T _element);
	bool Prazan() const;
	T Uzmi();
	T Vrh() const;

	enum SGreska { PRAZAN };

};

template<class T>
inline void Stek<T>::kopiraj(const Stek & s)
{
	Element* original = s.pokazivac;
	Element* p = nullptr;
	Element* q = nullptr;

	while (original)
	{
		if (q == nullptr)
			q = p = pokazivac = new Element(original->sadrzaj);
		else
		{
			q = p;
			p = new Element(original->sadrzaj);
			q->sledeci = p;
		}

		original = original->sledeci;
	}
}

template<class T>
inline void Stek<T>::premesti(Stek && s)
{
	pokazivac = s.pokazivac;
	s.pokazivac = nullptr;
}

template<class T>
inline void Stek<T>::obrisi()
{
	while (pokazivac)
	{
		Element* tmp = pokazivac->sledeci;
		delete pokazivac;
		pokazivac = tmp;
	}
}

template<class T>
inline Stek<T> & Stek<T>::operator=(const Stek & s)
{
	if (this != &s) { obrisi(); kopiraj(s); }
	return *this;
}

template<class T>
inline Stek<T> & Stek<T>::operator=(Stek && s)
{
	if (this != &s) { obrisi(); premesti(s); }
	return *this;
}

template<class T>
inline void Stek<T>::Dodaj(T _element)
{
	pokazivac = new Element(_element, pokazivac);
}

template<class T>
inline bool Stek<T>::Prazan() const
{
	return pokazivac == nullptr;
}

template<class T>
inline T Stek<T>::Uzmi()
{
	if (pokazivac == nullptr)
		throw PRAZAN;
	else
	{
		T rez = pokazivac->sadrzaj;

		Element* tmp = pokazivac;
		pokazivac = pokazivac->sledeci;
		delete tmp;

		return rez;
	}
}

template<class T>
inline T Stek<T>::Vrh() const
{
	if (pokazivac != nullptr)
		return pokazivac->sadrzaj;
	else
		throw PRAZAN;
}

#pragma endregion

#pragma region "AnalizatorPerformansi.cpp"

double AnalizatorPerformansi::FormirajSkup(Skup & skup, int velicina, int seme)
{
	PerformanceCalculator pc;
	double t = 0; // vreme

				  // merenje formiranja tri puta
	for (int i = 1; i <= 3; i++)
	{
		if (i != 1)
			skup.IsprazniSkup();

		pc.start();

		srand(seme);
		// dodavanje 'velicina' elemenata
		for (int k = 1; k <= velicina; k++)
		{
			// bolje performanse daje usvojeno resenje od komentarisanog 998 500/1 000 000
			int b = rand()*(RAND_MAX + 1) + rand();
			//(int)(rand() / ((double)RAND_MAX)*(2 * velicina)); // * (high - low) + low

			skup.UmetniElement(b);
		}

		pc.stop();
		t += pc.elapsedMillis();
	}

	return t / 3;
}

double AnalizatorPerformansi::Umetanje(Skup & skup, int novi)
{
	PerformanceCalculator pc;

	pc.start();
	skup.UmetniElement(novi);
	pc.stop();

	return pc.elapsedMillis();
}

double AnalizatorPerformansi::Brisanje(Skup & skup, int k)
{
	PerformanceCalculator pc;

	pc.start();
	skup.ObrisiElement(k);
	pc.stop();

	return pc.elapsedMillis();
}

double AnalizatorPerformansi::Pretraga(Skup & skup, int k, bool & postoji)
{
	PerformanceCalculator pc;
	double t = 0;

	for (int i = 1; i <= 3; i++)
	{
		pc.start();
		postoji = skup.PostojanjeElementa(k);
		pc.stop();
		t += pc.elapsedMillis();
	}

	return t / 3;
}

double AnalizatorPerformansi::BrisanjeOpsegaVrednosti(Skup & skup, int min, int max)
{
	PerformanceCalculator pc;

	pc.start();
	skup.ObrisiOpsegVrednosti(min, max);
	pc.stop();

	return pc.elapsedMillis();
}

double AnalizatorPerformansi::Razlika(Skup & s1, Skup & s2, int velicina)
{
	PerformanceCalculator pc;

	// formiranje elemenata drugog skupa
	for (int i = 1; i <= velicina; i++)
	{
		// 0 do 2 * 'velicina'
		int b = rand()*(RAND_MAX + 1) + rand();

		s2.UmetniElement(b);
	}

	double t = 0;

	for (int i = 1; i <= 3; i++)
	{
		pc.start();
		Skup* rez = s1 - s2;
		pc.stop();
		t += pc.elapsedMillis();
		delete rez;
	}

	return t / 3;
}

#pragma endregion

#pragma region "BSTSkup.cpp"

void BSTSkup::kopiraj(const BSTSkup & s)
{
	Cvor* p = s.koren;
	if (p == nullptr) // nema svrhe kopiranje
		return;

	Cvor* noviKoren, *tmp;

	Stek<Cvor*> stek1, stek2;

	// kopiranje korena
	noviKoren = new Cvor(p->kljuc); // const za rezultat
	tmp = noviKoren;				// promenljiva za obradu
	n++;

	stek1.Dodaj(p);
	stek2.Dodaj(tmp);

	// preorder kopiranje stabla
	while (!stek1.Prazan() && !stek2.Prazan())
	{
		p = stek1.Uzmi();	// originalno stablo
		tmp = stek2.Uzmi();	// kopirano stablo

		while (p != nullptr)
		{
			if (p->levi != nullptr)	// kopiranje levog podstabla
			{
				tmp->levi = new Cvor(p->levi->kljuc);
				n++;
			}
			if (p->desni != nullptr) // kopiranje desnog podstabla
			{
				tmp->desni = new Cvor(p->desni->kljuc);
				n++;
			}

			if (p->desni != nullptr) // dodavanje na stek desnog cvora
			{
				stek1.Dodaj(p->desni);
				stek2.Dodaj(tmp->desni);
			}

			// skretanje ulevo
			p = p->levi;
			tmp = tmp->levi;
		}
	}

	koren = noviKoren;
}

void BSTSkup::premesti(BSTSkup & s)
{
	koren = s.koren;
	n = s.n;

	s.koren = nullptr;
	s.n = 0;
}

// postorder brisanje stabla
void BSTSkup::obrisi()
{
	// niz posecenih cvorova; svaki cvor kada se prvi put poseti stavi se da bude false, 
	// a drugi put (kada treba da bude obrisan) njegov par u nizu posecenih jeste false
	bool *poseceni = new bool[n];
	int i = 0;

	/* stek cvorova sluzi za pamcenje cvorova, dok stek indeksa je stek na koji
	se dodaje i uzima u isto vreme kada se to radi i u prvom steku i prestavlja
	mesto za cuvanje indeksa niza posecenog cvorova (jer c jezici ne podrzavaju
	negaciju pokazivaca)	*/
	Stek<Cvor*> stek;
	Stek<int> stekIndeksa;

	Cvor* tmp = koren;

	// navigacija u levu stranu
	while (tmp != nullptr)
	{
		stek.Dodaj(tmp);
		stekIndeksa.Dodaj(i);
		poseceni[i++] = false;

		tmp = tmp->levi;
	}

	while (!stek.Prazan())
	{
		// uzimanje sa steka
		tmp = stek.Uzmi();
		int pTabela = stekIndeksa.Uzmi();
		i--;

		if (poseceni[pTabela] == false) // prva poseta
		{
			// dodavanje na stek drugi put
			stek.Dodaj(tmp);
			stekIndeksa.Dodaj(i);
			poseceni[i++] = true;

			// navigacija u desnog sina
			tmp = tmp->desni;

			// navigacija i dodavanje na levo
			while (tmp != nullptr)
			{
				stek.Dodaj(tmp);
				stekIndeksa.Dodaj(i);
				poseceni[i++] = false;

				tmp = tmp->levi;
			}
		}
		else // druga poseta
			delete tmp;
	}

	koren = nullptr;
	n = 0;
	delete poseceni;
}

BSTSkup::Cvor * BSTSkup::pronadiElement(int k, Cvor* koren) const
{
	Cvor* tmp = koren;

	// pretrazivanje
	while (tmp != nullptr)
	{
		// skretanje u levo i desno podstablo
		if (k == tmp->kljuc) // element je pronaden
			return tmp;
		else if (k < tmp->kljuc)
			tmp = tmp->levi;
		else
			tmp = tmp->desni;
	}

	return nullptr;
}

void BSTSkup::pisi(ostream & o) const
{
	Stek<Cvor*> stek;
	Cvor* tmp = koren;

	while (1)
	{
		while (tmp)
		{
			stek.Dodaj(tmp);
			tmp = tmp->levi;
		}

		if (!stek.Prazan())
		{
			tmp = stek.Uzmi();
			o << tmp->kljuc << ' ';
			tmp = tmp->desni;
		}
		else
			break;
	}
}

BSTSkup & BSTSkup::operator=(const BSTSkup & s)
{
	if (this != &s) { obrisi(); kopiraj(s); }

	return *this;
}

BSTSkup & BSTSkup::operator=(BSTSkup && s)
{
	if (this != &s) { obrisi(); premesti(s); }

	return *this;
}

bool BSTSkup::PostojanjeElementa(int k) const
{
	return pronadiElement(k, koren) != nullptr;
}

BSTSkup & BSTSkup::UmetniElement(int k)
{
	Cvor* p = koren;   // trenutni
	Cvor* q = nullptr; // prethodni cvor

					   // pretraga i pamcenje prethodnog cvora
	while (p != nullptr)
	{
		q = p;

		// skretanje u levo i desno podstablo
		if (k < p->kljuc)
			p = p->levi;
		else if (k > p->kljuc)
			p = p->desni;
		else // element je vec u skupu
			return *this;
	}

	Cvor* novi = new Cvor(k);

	// pakovanje novog cvora na prethodnik
	if (q == nullptr) // slucaj da je stablo prazno
		koren = novi;
	else
		if (k < q->kljuc)
			q->levi = novi;
		else
			q->desni = novi;

	// brojac broja elemenata
	n++;

	return *this;
}

BSTSkup & BSTSkup::ObrisiElement(int k)
{
	Cvor* p = koren;   // trenutni
	Cvor* q = nullptr; // prethodni

					   // pretrazivanje na zadati kljuc i pamcenje prethodnika
	while (p != nullptr && p->kljuc != k)
	{
		q = p;

		// skretanje u levo i desno podstablo
		if (k < p->kljuc)
			p = p->levi;
		else
			p = p->desni;
	}

	if (p == nullptr) // element za brisanje ne postoji u skupu
		return *this;

	Cvor* rp = nullptr; // posto se p brise, rp je cvor koji je sin od p

	if (p->levi == nullptr) // levi sin p ne postoji, rp je desni ako postoji
		rp = p->desni;
	else if (p->desni == nullptr) // desni sin p ne postoji, rp je levi jer postoji
		rp = p->levi;
	else // p ima i levog i desnog sina
	{
		Cvor* f = nullptr, *s = nullptr;

		f = p;
		rp = f->desni;
		s = rp->levi;

		while (s != nullptr)
		{
			f = rp;
			rp = s;
			s = rp->levi;
		}

		if (f != p)
		{
			f->levi = rp->desni;
			rp->desni = p->desni;
		}

		rp->levi = p->levi;
	}

	if (q == nullptr) // prethodnik ne postoji
		koren = rp;
	else if (p == q->levi) // p je bio levi sin oca
		q->levi = rp;
	else if (p == q->desni) // p je bio desni sin oca
		q->desni = rp;

	delete p;

	// brojac elemenata
	n--;

	return *this;
}

BSTSkup & BSTSkup::ObrisiOpsegVrednosti(int min, int max)
{
	if (min > max)
		throw INDEKS;

	for (int i = min; i <= max; i++)
		ObrisiElement(i);

	return *this;
}

void BSTSkup::IsprazniSkup()
{
	obrisi();
	koren = nullptr;
	n = 0;
}

BSTSkup * BSTSkup::Razlika(const Skup & s2) const
{
	BSTSkup* rezultat = new BSTSkup();

	Stek<Cvor*> stek1, stek2;

	try
	{
		Cvor* p1 = koren;
		Cvor* p2 = (dynamic_cast<const BSTSkup&>(s2)).koren;

		/* inorder obilazak stabla jer on jedini kod BST daje sortiran
		redosled cvorova, a procedura je zatim slicna kao liste -
		poredi se cvor po cvor */
		while (1)
		{
			while (p1 != nullptr) // dodavanje na stek iz p1 sve do krajnjeg levog cvora
			{
				stek1.Dodaj(p1);
				p1 = p1->levi;
			}

			while (p2 != nullptr) // dodavanje na stek iz p2 sve do krajnjeg levog cvora
			{
				stek2.Dodaj(p2);
				p2 = p2->levi;
			}

			if (!stek1.Prazan() && !stek2.Prazan())
			{
				if (stek1.Vrh()->kljuc < stek2.Vrh()->kljuc) // info(p1) < info(p2) - dodavanje elementa
				{
					p1 = stek1.Uzmi();
					rezultat->operator+=(p1->kljuc);

					p1 = p1->desni;
				}
				else if (stek1.Vrh()->kljuc == stek2.Vrh()->kljuc) // ima u oba - pomeranje na sledbenik
				{
					p1 = stek1.Uzmi();
					p2 = stek2.Uzmi();

					p1 = p1->desni;
					p2 = p2->desni;
				}
				else
				{
					p2 = stek2.Uzmi();

					p2 = p2->desni;
				}
			}
			else
				break;
		}

		// ispisivanje onoga sto je ostalo u steku br. 1 jer to spada u razliku skupova i to sa inorder 
		while (1)
		{
			while (p1 != nullptr) // dodavanje na stek iz p1 sve do krajnjeg levog cvora
			{
				stek1.Dodaj(p1);
				p1 = p1->levi;
			}

			if (!stek1.Prazan())
			{
				p1 = stek1.Uzmi();
				rezultat->operator+=(p1->kljuc);

				p1 = p1->desni;
			}
			else
				break;
		}

	}
	catch (const std::bad_cast&)
	{
		throw TIP;
	}

	return rezultat;
}

#pragma endregion

#pragma region "DCLLSkup.cpp"

void DCLLSkup::kopiraj(const DCLLSkup & ss)
{
	pZaglavlje = new Zaglavlje(0, nullptr, nullptr);

	Element *original = ss.pZaglavlje->sledeci;
	Element *prethodni = pZaglavlje;
	Element *trenutni = nullptr;

	// dodavanje elementa po element uz pamcenje zadnjeg pokazivaca
	while (original != ss.pZaglavlje)
	{
		trenutni = new Element(original->broj);

		trenutni->prethodni = prethodni;
		prethodni->sledeci = trenutni;

		prethodni = trenutni;
		original = original->sledeci;

		azurirajZaglavlje(DODAVANJE);
	}

	if (trenutni != nullptr) // zadnji element pokazuje na zaglavlje i obrnuto
	{
		trenutni->sledeci = pZaglavlje;
		pZaglavlje->prethodni = trenutni;
	}
	else // zaglavlje pokazuje samo na sebe jer nema elemenata
	{
		pZaglavlje->sledeci = pZaglavlje;
		pZaglavlje->prethodni = pZaglavlje;
	}
}

void DCLLSkup::premesti(DCLLSkup & s)
{
	pZaglavlje = s.pZaglavlje;
	s.pZaglavlje = nullptr;
}

void DCLLSkup::obrisi()
{
	Element* t = pZaglavlje->sledeci;

	while (t != pZaglavlje)
	{
		Element* sledeci = t->sledeci;
		delete t;
		t = sledeci;
	}

	delete pZaglavlje;
}

inline void DCLLSkup::inicijalizuj()
{
	// formiranje zaglavlja ulancane liste sa brojem 0 kao brojem clanova skupa
	pZaglavlje = new Zaglavlje(0, nullptr, nullptr);
	pZaglavlje->sledeci = pZaglavlje;
	pZaglavlje->prethodni = pZaglavlje;
}

inline void DCLLSkup::azurirajZaglavlje(Operacija op)
{
	// azuriranje broja elemenata u zaglavlju
	if (op == DODAVANJE)
		pZaglavlje->broj++;
	else
		pZaglavlje->broj--;
}

inline void DCLLSkup::dodajNakon(Element * p, int k)
{
	Element* novi = new Element(k);
	Element* sledeci = p->sledeci;

	novi->prethodni = p;
	novi->sledeci = sledeci;

	p->sledeci = novi;
	sledeci->prethodni = novi;

	azurirajZaglavlje(DODAVANJE);
}

inline void DCLLSkup::dodajPre(Element * p, int k)
{
	Element* novi = new Element(k);
	Element* prethodni = p->prethodni;

	novi->prethodni = prethodni;
	novi->sledeci = p;

	p->prethodni = novi;
	prethodni->sledeci = novi;

	azurirajZaglavlje(DODAVANJE);
}

void DCLLSkup::pisi(ostream & o) const
{
	Element* tmp = pZaglavlje->sledeci;

	while (tmp != pZaglavlje)
	{
		o << tmp->broj << ' ';
		tmp = tmp->sledeci;
	}
}

DCLLSkup & DCLLSkup::operator=(const DCLLSkup & s)
{
	if (this != &s) { obrisi(); kopiraj(s); }

	return *this;
}

DCLLSkup & DCLLSkup::operator=(DCLLSkup && s)
{
	if (this != &s) { obrisi(); premesti(s); }

	return *this;
}

bool DCLLSkup::PostojanjeElementa(int k) const
{
	if (pZaglavlje->broj == 0) // skup je prazan, nema svrhe da se pretrazuje
		return false;
	else
	{
		Element* el = pZaglavlje->sledeci;

		while (el != pZaglavlje)
		{
			if (el->broj == k)
				return true;

			el = el->sledeci;
		}

		return false;
	}
}

DCLLSkup & DCLLSkup::UmetniElement(int k)
{
	Element* trenutni = pZaglavlje;

	// prvi slucaj je slucaj da je lista prazna
	if (trenutni->sledeci == pZaglavlje)
		dodajNakon(trenutni, k);
	else
	{
		// prelazak na prvi element
		trenutni = trenutni->sledeci;

		// pretrazivanje na element 'k'
		while (trenutni != pZaglavlje && trenutni->broj < k)
			trenutni = trenutni->sledeci;

		// ako postoji nista, u suprotnom dodavanje
		if (trenutni->broj != k)
			// provera da sledeci cvor nije zaglavlje cije je polje za broj elemenata jednako 'k'
			if (trenutni->sledeci == pZaglavlje || trenutni->sledeci->broj != k)
				dodajPre(trenutni, k);
	}

	return *this;
}

DCLLSkup & DCLLSkup::ObrisiElement(int k)
{
	Element *trenutni = pZaglavlje->sledeci;

	// nebrisanje zaglavlja
	if (trenutni != pZaglavlje)
	{
		while (trenutni != pZaglavlje && trenutni->broj < k)
			trenutni = trenutni->sledeci;

		if (trenutni != pZaglavlje && trenutni->broj == k)
		{
			Element* tmp = trenutni;

			// prespojavanje
			trenutni->prethodni->sledeci = trenutni->sledeci;
			trenutni->sledeci->prethodni = trenutni->prethodni;

			azurirajZaglavlje(BRISANJE);

			delete tmp;
		}
	}

	return *this;
}

DCLLSkup & DCLLSkup::ObrisiOpsegVrednosti(int min, int max)
{
	if (min > max)
		throw INDEKS;

	Element* tmp = pZaglavlje->sledeci;
	int i = min;

	while (tmp != pZaglavlje && i >= min && i <= max)
	{
		if (tmp->broj == i)
		{
			// brisanje
			Element* trenutni = tmp;

			// prespajanje
			trenutni->prethodni->sledeci = trenutni->sledeci;
			trenutni->sledeci->prethodni = trenutni->prethodni;

			azurirajZaglavlje(BRISANJE);

			tmp = tmp->sledeci;
			i++;

			delete trenutni;
		}
		else if (tmp->broj > i) // i ne postoji
			i++;
		else // tmp->broj < i   // premotavanje
			tmp = tmp->sledeci;
	}

	return *this;
}

int DCLLSkup::BrojElemenata() const
{
	return pZaglavlje->broj;
}

void DCLLSkup::IsprazniSkup()
{
	obrisi();
	inicijalizuj();
}

DCLLSkup* DCLLSkup::Razlika(const Skup & s2) const
{
	DCLLSkup* rezultat = new DCLLSkup();

	Element* p1 = pZaglavlje->sledeci;

	try
	{
		Element* p2zag = (dynamic_cast<const DCLLSkup&>(s2)).pZaglavlje;
		Element* p2 = p2zag->sledeci;

		// trazenje sta ima u s1, a nema u s2
		while (p1 != pZaglavlje && p2 != p2zag)
		{
			if (p1->broj < p2->broj)
			{
				rezultat->operator+=(p1->broj);
				p1 = p1->sledeci;
			}
			else if (p1->broj == p2->broj)
			{
				p1 = p1->sledeci;
				p2 = p2->sledeci;
			}
			else // p1->broj > p2->broj
			{
				p2 = p2->sledeci;
			}
		}

		// kopiranje ostatka u p1 skupu jer i to cini razliku
		while (p1 != pZaglavlje)
		{
			rezultat->operator+=(p1->broj);
			p1 = p1->sledeci;
		}
	}
	catch (const std::bad_cast&)
	{
		throw TIP;
	}

	return rezultat;
}

#pragma endregion

#pragma region "Skup.cpp"

Skup & Skup::operator+=(int k)
{
	return UmetniElement(k);
}

Skup & Skup::operator-=(int k)
{
	return ObrisiElement(k);
}

Skup * operator-(const Skup & s1, const Skup & s2)
{
	return s1.Razlika(s2);
}

ostream & operator<<(ostream & o, const Skup & s)
{
	s.pisi(o);
	return o;
}

#pragma endregion

#pragma region "Main.cpp"

int main()
{
	int seme = (int)time(NULL);
	Skup* skup = nullptr;

	Skup* korisnickiSkup = nullptr;
	int vrstaSkupa;

	while (1)
	{
		cout << endl << "Izaberite opciju: " << endl;
		cout << "--------------------------------" << endl;
		cout << "1. Merenje performansi izvrsavanja" << endl;
		cout << "--------------------------------" << endl;
		cout << "2. Formiranje praznog skupa" << endl;
		cout << "3. Umetanje elementa" << endl;
		cout << "4. Brisanje elementa" << endl;
		cout << "5. Brisanje opsega elemenata" << endl;
		cout << "6. Pretrazivanje na element" << endl;
		cout << "7. Broj elemenata" << endl;
		cout << "8. Praznjenje skupa" << endl;
		cout << "9. Ispisivanje skupa" << endl;
		cout << "10. Razlika skupova" << endl;
		cout << "--------------------------------" << endl;
		cout << "0. Kraj rada" << endl;
		cout << "--------------------------------" << endl << endl;

		int opcija;
		cin >> opcija;

		if (opcija == 1) // testiranje performansi
		{
			cout << "- Za skup predstavljen dvostruko ulancanom listom unesite 1" << endl;
			cout << "- Za skup predstavljen binarnim stablom pretrazivanja unesite 2" << endl;

			int vrsta;
			cin >> vrsta;

			if (skup != nullptr)
			{
				delete skup;
				skup = nullptr;
			}

			if (vrsta == 1)
				skup = new DCLLSkup();
			else if (vrsta == 2)
				skup = new BSTSkup();
			else
				continue;

			cout << "Unesite velicinu skupa: ";
			int velicina;
			cin >> velicina;

			if (velicina <= 0)
				continue;

			cout << "Prosecno vreme formiranja skupa je: ";
			cout << AnalizatorPerformansi::FormirajSkup(*skup, velicina, seme) << " ms" << endl;

			while (1)
			{
				cout << "Izaberite operaciju koju zelite da analizirate: " << endl;
				cout << "1. Umetanje" << endl;
				cout << "2. Brisanje" << endl;
				cout << "3. Pretraga" << endl;
				cout << "4. Brisanje opsega vrednosti" << endl;
				cout << "5. Razlika" << endl;
				cout << "6. Povratak na glavni meni" << endl;

				int operacija;
				cin >> operacija;

				if (operacija == 1) // umetanje
				{
					cout << "Unesite element:" << endl;
					int zaUmetanje;
					cin >> zaUmetanje;

					if (skup->PostojanjeElementa(zaUmetanje))
						cout << "Greska. Uneti element vec postoji u skupu." << endl;
					else
					{
						cout << "Vreme umetanja elementa '" << zaUmetanje << "' je: " << endl;
						cout << AnalizatorPerformansi::Umetanje(*skup, zaUmetanje) << " ms" << endl;
					}
				}
				else if (operacija == 2) // brisanje
				{
					cout << "Unesite element:" << endl;
					int zaBrisanje;
					cin >> zaBrisanje;

					if (!skup->PostojanjeElementa(zaBrisanje))
						cout << "Greska. Uneti element ne postoji u skupu." << endl;
					else
					{
						cout << "Vreme brisanja elementa '" << zaBrisanje << "' je: " << endl;
						cout << AnalizatorPerformansi::Brisanje(*skup, zaBrisanje) << " ms" << endl;
					}
				}
				else if (operacija == 3) // pretraga
				{
					cout << "Unesite element:" << endl;
					int zaPretragu;
					cin >> zaPretragu;
					bool postoji;

					cout << "Vreme pretrage elementa '" << zaPretragu << "' je: " << endl;
					cout << AnalizatorPerformansi::Pretraga(*skup, zaPretragu, postoji) << " ms" << endl;
					if (postoji)
						cout << "Uneti element postoji" << endl;
					else
						cout << "Uneti element ne postoji" << endl;
				}
				else if (operacija == 4) // brisanje opsega vrednosti
				{
					cout << "Unesite min i max u jednom redu:" << endl;
					int min, max;
					cin >> min;
					cin >> max;

					cout << "Vreme brisanja skupa elementa '" << min << "' do '" << max << "' je: " << endl;
					try { cout << AnalizatorPerformansi::BrisanjeOpsegaVrednosti(*skup, min, max) << " ms" << endl; }
					catch (...) { cout << "Indeksi nisu valjani"; }
				}
				else if (operacija == 5) // razlika
				{
					Skup* s2 = nullptr;

					if (vrsta == 1)
						s2 = new DCLLSkup();
					else
						s2 = new BSTSkup();

					cout << "Vreme odredivanja preseka razlike je: " << endl;
					cout << AnalizatorPerformansi::Razlika(*skup, *s2, velicina) << " ms" << endl;

					delete s2;
				}
				else // povratak na glavni meni
				{
					delete skup;
					skup = nullptr;
					break;
				}
			}
		}
		else if (opcija == 2) // formiranje skupa
		{
			cout << "- Za skup predstavljen dvostruko ulancanom listom unesite 1" << endl;
			cout << "- Za skup predstavljen binarnim stablom pretrazivanja unesite 2" << endl;
			cout << "* Unosom bilo koje druge vrednosti ce skup biti obrisan" << endl;
			cin >> vrstaSkupa;

			if (korisnickiSkup != nullptr)
			{
				delete korisnickiSkup;
				korisnickiSkup = nullptr;
			}

			if (vrstaSkupa == 1)
			{
				korisnickiSkup = new DCLLSkup();
				cout << "Skup je napravljen." << endl;
			}
			else if (vrstaSkupa == 2)
			{
				korisnickiSkup = new BSTSkup();
				cout << "Skup je napravljen." << endl;
			}
			else
			{
				continue;
			}
		}
		else if (opcija == 3) // umetanje elemenata
		{
			if (korisnickiSkup != nullptr)
			{
				cout << "- Za unos elementa sa standardnog ulaza unesite 1" << endl;
				cout << "- Za uvoz elemenata iz fajla unesite 2" << endl;

				int ulaz;
				cin >> ulaz;

				if (ulaz == 1) // direktno
				{
					cout << "Unesite element:" << endl;
					int x;
					cin >> x;

					korisnickiSkup->UmetniElement(x);
					cout << "Element je umetnut." << endl;
				}
				else if (ulaz == 2) // fajl
				{
					cout << "Unesite ime datoteke (absolute path): " << endl;
					string datoteka;
					cin >> datoteka;

					ifstream fajl;
					fajl.open(datoteka);
					if (!fajl.is_open())
					{
						cout << "Greksa pri citanju fajla." << endl;
						continue;
					}

					bool prekid = false;
					while (!fajl.eof() && !prekid)
					{
						try
						{
							string red;
							getline(fajl, red);
							int broj = stoi(red);

							korisnickiSkup->UmetniElement(broj);
						}
						catch (...)
						{
							cout << "Greksa pri pretvaranju niske u broj. Operacija je prekinuta." << endl;
							fajl.close();
							prekid = true;
						}
					}
					if (prekid)
						continue;

					fajl.close();

					cout << "Uspesno umetanje fajla." << endl;
				}
				else
					continue;
			}
			else
				cout << "Skup nije napravljen. Izaberite opciju 2 najpre." << endl;
		}
		else if (opcija == 4) // brisanje
		{
			if (korisnickiSkup != nullptr)
			{
				cout << "Unesite element:" << endl;
				int x;
				cin >> x;

				korisnickiSkup->ObrisiElement(x);
				cout << "Element je obrisan ako je postojao." << endl;
			}
			else
				cout << "Skup nije napravljen. Izaberite opciju 2 najpre." << endl;
		}
		else if (opcija == 5) // brisanje opsega
		{
			if (korisnickiSkup != nullptr)
			{
				cout << "Unesite min i max u jednom redu:" << endl;
				int min, max;
				cin >> min >> max;

				try
				{
					korisnickiSkup->ObrisiOpsegVrednosti(min, max);
					cout << "Elementi koji su postojali u zadatom opsegu su obrisani." << endl;
				}
				catch (...) { cout << "Indeksi nisu valjani" << endl; }
			}
			else
				cout << "Skup nije napravljen. Izaberite opciju 2 najpre." << endl;
		}
		else if (opcija == 6) // pretrazivanje
		{
			if (korisnickiSkup != nullptr)
			{
				cout << "Unesite element:" << endl;
				int x;
				cin >> x;

				bool postoji = korisnickiSkup->PostojanjeElementa(x);
				if (postoji)
					cout << "Trazeni element postoji." << endl;
				else
					cout << "Trazeni element ne postoji." << endl;
			}
			else
				cout << "Skup nije napravljen. Izaberite opciju 2 najpre." << endl;
		}
		else if (opcija == 7) // broj elemenata skupa
		{
			if (korisnickiSkup != nullptr)
			{
				cout << "Skup ima " << korisnickiSkup->BrojElemenata() << " elemenata." << endl;
			}
			else
				cout << "Skup nije napravljen. Izaberite opciju 2 najpre." << endl;
		}
		else if (opcija == 8) // praznjenje
		{
			if (korisnickiSkup != nullptr)
			{
				korisnickiSkup->IsprazniSkup();
				cout << "Skup je ispraznjen." << endl;
			}
			else
				cout << "Skup nije napravljen. Izaberite opciju 2 najpre." << endl;
		}
		else if (opcija == 9) // ispisivanje
		{
			if (korisnickiSkup != nullptr)
			{
				cout << "- Za ispis elemenata na standardni izlaz unesite 1" << endl;
				cout << "- Za ispis elemenata u fajla unesite 2" << endl;

				int ulaz;
				cin >> ulaz;

				if (ulaz == 1) // direktno
				{
					cout << *korisnickiSkup << endl;
				}
				else if (ulaz == 2) // u fajl
				{
					cout << "Unesite ime datoteke (absolute path): " << endl;
					string datoteka;
					cin >> datoteka;

					ofstream fajl;
					fajl.open(datoteka);
					if (!fajl.is_open())
					{
						cout << "Greksa pri otvaranju fajla." << endl;
						continue;
					}

					ostringstream ss;
					ss << *korisnickiSkup;
					string zaIspis = ss.str();
					zaIspis = zaIspis.substr(0, zaIspis.length() - 1);
					replace(zaIspis.begin(), zaIspis.end(), ' ', '\n');

					fajl << zaIspis;
					fajl.close();

					cout << "Uspesan ispis u fajl." << endl;
				}
				else
					continue;
			}
			else
				cout << "Skup nije napravljen. Izaberite opciju 2 najpre." << endl;
		}
		else if (opcija == 10) // razlika
		{
			if (korisnickiSkup != nullptr)
			{
				cout << "- Za unos elemenata sa standardnog ulaza unesite 1" << endl;
				cout << "- Za uvoz elemenata iz fajla unesite 2" << endl;

				int ulaz;
				cin >> ulaz;

				Skup* zaRazliku = nullptr;

				if (ulaz == 1) // direktno
				{
					int brojElemenata;
					cout << "Unesite broj elemenata skupa za razliku: " << endl;
					cin >> brojElemenata;
					if (brojElemenata < 1)
						continue;

					if (vrstaSkupa == 1)
						zaRazliku = new DCLLSkup();
					else
						zaRazliku = new BSTSkup();

					cout << "Unosite element po element u novom redu: " << endl;
					for (int i = 0; i < brojElemenata; i++)
					{
						int kk;
						cin >> kk;
						zaRazliku->operator+=(kk);
					}
				}
				else if (ulaz == 2)
				{
					if (vrstaSkupa == 1)
						zaRazliku = new DCLLSkup();
					else
						zaRazliku = new BSTSkup();

					cout << "Unesite ime datoteke (absolute path): " << endl;
					string datoteka;
					cin >> datoteka;

					ifstream fajl;
					fajl.open(datoteka);
					if (!fajl.is_open())
					{
						cout << "Greksa pri citanju fajla." << endl;
						continue;
					}

					bool prekid = false;
					while (!fajl.eof() && !prekid)
					{
						try
						{
							string red;
							getline(fajl, red);
							int broj = stoi(red);

							zaRazliku->UmetniElement(broj);
						}
						catch (...)
						{
							cout << "Greksa pri pretvaranju niske u broj. Operacija je prekinuta." << endl;
							fajl.close();
							prekid = true;
						}
					}
					if (prekid)
						continue;

					fajl.close();
				}
				else
					continue;


				Skup* razlika = *korisnickiSkup - *zaRazliku;
				cout << "Razlika: " << *razlika << endl;

				delete razlika;
				delete zaRazliku;
			}
			else
				cout << "Skup nije napravljen. Izaberite opciju 2 najpre." << endl;
		}
		else if (opcija == 0) // izlaz
		{
			exit(0);
		}
	}
}

#pragma endregion
