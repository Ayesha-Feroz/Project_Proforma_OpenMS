#pragma once

#include "ProFormaTagGroup.h"

namespace ProForma {
	/**
	 * \class ProFormaTagGroupChangingValue
	 *
	 * \brief Derived class to those with changing value.
	 *
	 */

	class ProFormaTagGroupChangingValue: public ProFormaTagGroup {
	public:
        /** \brief  Initializes a new instance of the ProFormaTagGroup class
		  * \param  name The name.
          * \param  key The key.
          * \param  value The value.
          * \param  members The members.
		  * \return void
		  */
        ProFormaTagGroupChangingValue(std::string name, ProFormaKey key, ProFormaEvidenceType evidenceType,
                std::list<ProFormaMembershipDescriptor> members) : ProFormaTagGroup(name, key, evidenceType, "", members), _keyFlux(ProFormaKey::None), _evidenceTypeFlux(ProFormaEvidenceType::None), _valueFlux("")
        {
            SetKeyFlux(key);
            SetEvidenceFlux(evidenceType);
            SetValueFlux("");
            this->_isChanging = true;
        }

        // New setters and getters
        /** \brief The key getter and setter. */
        ProFormaKey KeyFlux() { return _key; }
        void SetKeyFlux(ProFormaKey key) { if(key != ProFormaKey::None) _key = key; }

        /** \brief The type of the evidence getter and setter. */
        ProFormaEvidenceType EvidenceFlux() { return _evidenceType; }
        void SetEvidenceFlux(ProFormaEvidenceType evidenceType) { if(evidenceType != ProFormaEvidenceType::None) _evidenceType = evidenceType; }

        /** \brief The value getter and setter. */
        std::string ValueFlux() { return _value;  }
        void SetValueFlux(std::string value) { if(value.length())_value = value; }
    private:
        ProFormaKey _keyFlux;
        ProFormaEvidenceType _evidenceTypeFlux;
        std::string _valueFlux;
	};
}
