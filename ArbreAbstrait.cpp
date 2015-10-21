#include <stdlib.h>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : m_instructions() {
}

int NoeudSeqInst::executer() {
  for (unsigned int i = 0; i < m_instructions.size(); i++)
    m_instructions[i]->executer(); // on exécute chaque instruction de la séquence
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudSeqInst::ajoute(Noeud* instruction) {
  if (instruction!=nullptr) m_instructions.push_back(instruction);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression)
: m_variable(variable), m_expression(expression) {
}

int NoeudAffectation::executer() {
  int valeur = m_expression->executer(); // On exécute (évalue) l'expression
  ((SymboleValue*) m_variable)->setValeur(valeur); // On affecte la variable
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit)
: m_operateur(operateur), m_operandeGauche(operandeGauche), m_operandeDroit(operandeDroit) {
}

int NoeudOperateurBinaire::executer() {
  int og, od, valeur;
  if (m_operandeGauche != nullptr) og = m_operandeGauche->executer(); // On évalue l'opérande gauche
  if (m_operandeDroit != nullptr) od = m_operandeDroit->executer(); // On évalue l'opérande droit
  // Et on combine les deux opérandes en fonctions de l'opérateur
  if (this->m_operateur == "+") valeur = (og + od);
  else if (this->m_operateur == "-") valeur = (og - od);
  else if (this->m_operateur == "*") valeur = (og * od);
  else if (this->m_operateur == "==") valeur = (og == od);
  else if (this->m_operateur == "!=") valeur = (og != od);
  else if (this->m_operateur == "<") valeur = (og < od);
  else if (this->m_operateur == ">") valeur = (og > od);
  else if (this->m_operateur == "<=") valeur = (og <= od);
  else if (this->m_operateur == ">=") valeur = (og >= od);
  else if (this->m_operateur == "et") valeur = (og && od);
  else if (this->m_operateur == "ou") valeur = (og || od);
  else if (this->m_operateur == "non") valeur = (!og);
  else if (this->m_operateur == "/") {
    if (od == 0) throw DivParZeroException();
    valeur = og / od;
  }
  return valeur; // On retourne la valeur calculée
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSi::NoeudInstSi(Noeud* condition, Noeud* sequence, Noeud* sinon,vector<Noeud *>sequence_sinonsi,vector<Noeud *>condition_sinonsi)
: m_condition(condition), m_sequence(sequence), seq_sinon(sinon), m_condi_sinonsi(condition_sinonsi), m_seq_sinonsi(sequence_sinonsi) {
}

int NoeudInstSi::executer() {
    bool si;
    bool sinon;
  if (m_condition->executer()){
      m_sequence->executer();
      si=true;
  }
  else if(m_condi_sinonsi.size()>0 && m_seq_sinonsi.size()>0){
    int i=0;
    while(i<m_condi_sinonsi.size()){
            if(m_condi_sinonsi.at(i)->executer()){
                m_seq_sinonsi.at(i)->executer();
                sinon = true;
                
            i=m_condi_sinonsi.size();
            }
            else
             i++;
    }
    
    
  }
      if(!si && !sinon )
    seq_sinon->executer();
  
 
  return 0; // La valeur renvoyée ne représente rien !
}

NoeudInstTantQue::NoeudInstTantQue(Noeud* condition, Noeud* sequence)
:m_condition(condition), m_sequence(sequence){
    
}

int NoeudInstTantQue::executer(){
 while (m_condition->executer()) 
     m_sequence->executer();
 
 
  return 0; // La valeur renvoyée ne représente rien !
}


NoeudInstPour::NoeudInstPour(Noeud* affect1, Noeud* condition, Noeud* affect2, Noeud* sequence)
:m_affectation1(affect1),m_condition(condition),m_sequence(sequence),m_affectation2(affect2){
    
}



int NoeudInstPour::executer(){
    if(m_affectation1 == nullptr && m_affectation2 == nullptr){
        for(;m_condition->executer();)
        m_sequence->executer();    
    }
    
    else if (m_affectation1 != nullptr && m_affectation2 == nullptr){
        for(m_affectation1->executer();m_condition->executer();)
            m_sequence->executer();
        
        
    }
    else if (m_affectation1 == nullptr && m_affectation2 != nullptr){
        for(;m_condition->executer();m_affectation2->executer()){
            m_sequence->executer();
        }
        
    }

    else if (m_affectation1 != nullptr && m_affectation2 != nullptr){
        
        for(m_affectation1->executer();m_condition->executer();m_affectation2->executer()){
            m_sequence->executer();
        }
       

    }
     return 0;       
}