//
// Created by simon on 23/09/2019.
//

#ifndef LABO01_SEARCHTREE_H
#define LABO01_SEARCHTREE_H

#include <cstdlib>
#include <iostream>
#include <queue>
#include <random>
#include <memory>
#include <functional>
#include <fstream>
#include <cassert>
#include <sstream>

using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::unique_ptr;
using std::cerr;
using std::pair;
using std::string;
/**********************************************************************

   Class: Zoekboom

   beschrijving: Binaire Zoekboom waarin duplicaatsleutels wel of niet zijn toegestaan.

***************************************************************************/

template <class Sleutel, class Data>
class zoekKnoop;

template <class Sleutel, class Data>
class Zoekboom : public unique_ptr<zoekKnoop<Sleutel, Data>> {
	//....move en copy. Noot: als er geen copy nodig is, zet hem beste op delete.
public:
	void inorder(std::function<void(const zoekKnoop<Sleutel, Data>&)> bezoek) const;
	void schrijf(ostream& os) const;
	void teken(const char* bestandsnaam);
	string tekenrec(ostream& uit, int& knoopteller);
	Zoekboom<Sleutel, Data>& operator=(zoekKnoop<Sleutel, Data>&& z);
	Zoekboom(unique_ptr<zoekKnoop<Sleutel, Data>>&& ptr) : unique_ptr<zoekKnoop<Sleutel, Data>>(std::move(ptr)) {}

	void roteer(bool n_rechts);
	void maakOnevenwichtig();

	Zoekboom() {}

	//te implementeren
	bool repOK() const;
	int geefDiepte();
	// geefBoomBovenKnoop: gegeven een knooppointer, wele boom wijst naar de knoop
	// preconditie: knoop moet een naar een geldige knoop wijzen.
	Zoekboom<Sleutel, Data>* geefBoomBovenKnoop(zoekKnoop<Sleutel, Data>& knoopptr);
	virtual zoekKnoop<Sleutel, Data>* voegtoe(const Sleutel& sleutel, const Data& data, bool dubbelsToestaan = false);

protected:
	//zoekfunctie zoekt sleutel en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
	//ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
	//noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
	void zoek(const Sleutel& sleutel, zoekKnoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats);
};

template <class Sleutel, class Data>
class zoekKnoop {
	friend class Zoekboom<Sleutel, Data>;
public:
	zoekKnoop() :ouder(0) {}
	zoekKnoop(const Sleutel& sl, const Data& d) :sleutel{ sl }, data(d), ouder(0){};
	zoekKnoop(Sleutel&& sl, Data&& d) :sleutel{ move(sl) }, data(move(d)), ouder(0){};
	Zoekboom<Sleutel, Data>& geefKind(bool links);
	Sleutel sleutel;
	Data data;
	zoekKnoop<Sleutel, Data>* ouder;
	Zoekboom<Sleutel, Data> links, rechts;
};


/*****************************************************************************

	Implementatie

*****************************************************************************/

// own implementation
template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>& Zoekboom<Sleutel, Data>::operator=(zoekKnoop<Sleutel, Data>&& z) {
	unique_ptr<zoekKnoop<Sleutel, Data>>::operator=(move(z));
	return *this;
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::roteer(bool n_rechts) {

	// niveau naar boven want je moet unique pointers  hebben om te swappen, ouder pointers moet je wel fixen achteraf
	/*zoekKnoop<Sleutel, Data>* ouder_ptr = this->ouder; // op unique pointer geen move

	zoekKnoop<Sleutel, Data> linkerKind = move((*this)->geefKind(n_rechts)); // geeft het linker kind terug
	swap(linkerKind->ouder, (*this)->ouder);

	linkerKind->rechts;*/

    // Hoofdknoop mag geen nullptr zijn
    assert(*this);
    // Tijdelijk L bijhouden
    Zoekboom<Sleutel, Data> L = move(this->get()->geefKind(n_rechts));
    // Andere knoop moet ook aanwezig zijn
    assert(L);

    // De ouder van de bovenste wordt de ouder van de onderste
    L->ouder = this->get()->ouder;

    // Het richting kind van de onderste het tegen richting kind maken van de
    // bovenste
    this->get()->geefKind(n_rechts) = move(L->geefKind(!n_rechts));

    // Indien er een kind was, de ouderpointer juist zetten
    if(this->get()->geefKind(n_rechts)){
        this->get()->geefKind(n_rechts)->ouder = this->get();
    }

    // Bovenste wordt richting kind van onderste
    L->geefKind(!n_rechts) = move(*this);
    // Ouder pointer juistzetten
    L->geefKind(!n_rechts)->ouder = L.get();


    // Hoofdwijzer juist zetten
    *this = move(L);
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::maakOnevenwichtig() {

}

// end of implementation

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::inorder(std::function<void(const zoekKnoop<Sleutel, Data>&)> bezoek) const {
	if (*this) {
		(*this)->links.inorder(bezoek);
		bezoek(**this);
		(*this)->rechts.inorder(bezoek);
	};
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::schrijf(ostream& os) const {
	inorder([&os](const zoekKnoop<Sleutel, Data>& knoop) {
		os << "(" << knoop.sleutel << " -> " << knoop.data << ")";
		os << "\n  Linkerkind: ";
		if (knoop.links)
			os << knoop.links->sleutel;
		else
			os << "-----";
		os << "\n  Rechterkind: ";
		if (knoop.rechts)
			os << knoop.rechts->sleutel;
		else
			os << "-----";
		os << "\n";
		});
}

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::teken(const char* bestandsnaam) {
	ofstream uit(bestandsnaam);
	assert(uit);
	int knoopteller = 0;//knopen moeten een eigen nummer krijgen.
	uit << "digraph {\n";
	this->tekenrec(uit, knoopteller);
	uit << "}";
}

template <class Sleutel, class Data>
int Zoekboom<Sleutel, Data>::geefDiepte() {

	if ((*this) == nullptr) {
		return 0;
	}

	int l_diepte = 0;
	int r_diepte = 0;
	if ((*this)->links) {
		l_diepte = (*this)->links.geefDiepte();
	}
	if ((*this)->rechts) {
		r_diepte = (*this)->rechts.geefDiepte();
	}

	return 1 + std::max(l_diepte, r_diepte);
}

template <class Sleutel, class Data>
string Zoekboom<Sleutel, Data>::tekenrec(ostream& uit, int& knoopteller) {
	ostringstream wortelstring;
	wortelstring << '"' << ++knoopteller << '"';
	if (!*this) {
		uit << wortelstring.str() << " [shape=point];\n";
	}
	else {
		uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << ":" << (*this)->data << "\"]";
		uit << ";\n";
		string linkskind = (*this)->links.tekenrec(uit, knoopteller);
		string rechtskind = (*this)->rechts.tekenrec(uit, knoopteller);
		uit << wortelstring.str() << " -> " << linkskind << ";\n";
		uit << wortelstring.str() << " -> " << rechtskind << ";\n";
	};
	return wortelstring.str();
}

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>* Zoekboom<Sleutel, Data>::geefBoomBovenKnoop(zoekKnoop<Sleutel, Data>& knoop) {
	if (knoop.ouder == 0)
		return this;
	else
		if (knoop.ouder->links.get() == &knoop)
			return &(knoop.ouder->links);
		else
			return &(knoop.ouder->rechts);
}

template <class Sleutel, class Data>
zoekKnoop<Sleutel, Data>* Zoekboom<Sleutel, Data>::voegtoe(const Sleutel& sleutel, const Data& data, bool dubbelsToestaan) {
	zoekKnoop<Sleutel, Data>* ouder;
	Zoekboom<Sleutel, Data>* plaats;
	Zoekboom<Sleutel, Data>::zoek(sleutel, ouder, plaats);
	if (dubbelsToestaan)
		while (*plaats)
			(*plaats)->geefKind(random() % 2).zoek(sleutel, ouder, plaats);
	if (!*plaats) {
		Zoekboom<Sleutel, Data> nieuw =
			std::make_unique<zoekKnoop<Sleutel, Data> >(sleutel, data);
		nieuw->ouder = ouder;
		*plaats = move(nieuw);
	}

	return plaats->get();
}


template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::zoek(const Sleutel& sleutel, zoekKnoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats) {
	plaats = this;
	ouder = 0;
	while (*plaats && (*plaats)->sleutel != sleutel) {
		ouder = plaats->get();
		if ((*plaats)->sleutel < sleutel)
			plaats = &(*plaats)->rechts;
		else
			plaats = &(*plaats)->links;
	};
};

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>& zoekKnoop<Sleutel, Data>::geefKind(bool linkerkind) {
	if (linkerkind)
		return links;
	else
		return rechts;
};


#endif //LABO01_SEARCHTREE_H
