#pragma once

#include <list>
#include <string>

#include "ProFormaMembershipDescriptor.h"
#include "ProFormaKey.h"

namespace ProForma {
	/**
	 * \class ProFormaTagGroup
	 *
	 * \brief A tag that is spread across multiple distinct sites.
	 *
	 */

	class ProFormaTagGroup : public IProFormaDescriptor {
	public:
        /** \brief  Initializes a new instance of the ProFormaTagGroup class
		  * \param  name The name.
          * \param  key The key.
          * \param  value The value.
          * \param  members The members.
		  * \return void
		  */
        ProFormaTagGroup(std::string name, ProFormaKey key, std::string value, std::list<ProFormaMembershipDescriptor> members)
            : ProFormaTagGroup(name, key, ProFormaEvidenceType::None, value, members) { }

        /** \brief  Initializes a new instance of the ProFormaTagGroup class
		  * \param  name The name.
          * \param  key The key.
          * \param  evidenceType  Type of the evidence.
          * \param  value The value.
          * \param  members The members.
		  * \return void
		  */
        ProFormaTagGroup(std::string name, ProFormaKey key, ProFormaEvidenceType evidenceType, std::string value,
            std::list<ProFormaMembershipDescriptor> members)
        {
            this->_name = name;
            this->_key = key;
            this->_evidenceType = evidenceType;
            this->_value = value;
            this->_members = members;
            this->_isChanging = false;
        }

        /** \brief  Copy constructor for ProFormaTagGroup class
		  * \param  group Group to be copied
		  * \return void
		  */
        ProFormaTagGroup(const ProFormaTagGroup& group) : ProFormaTagGroup(group._name, group._key, group._evidenceType, group._value, group._members)
        {
        }

        ProFormaTagGroup &operator=(ProFormaTagGroup &group) {
            if(this != &group) {
                this->_name = group.Name();
                this->_key = group.Key();
                this->_evidenceType = group.EvidenceType();
                this->_value = group.Value();
                this->_members = group.Members();
                this->_isChanging = group.IsChanging();
            }
            return *this;
        }

        /** \brief  The name of the group. */
        std::string Name() { return _name; }

        /** \brief The key getter and setter. */
        ProFormaKey Key() { return _key; }
        void SetKey(ProFormaKey key) { _key = key; }

        /** \brief The type of the evidence getter and setter. */
        ProFormaEvidenceType EvidenceType() { return _evidenceType; }
        void SetEvidenceType(ProFormaEvidenceType evidenceType) { _evidenceType = evidenceType; }

        /** \brief The value getter and setter. */
        std::string Value() { return _value;  }
        void SetValue(std::string value) { _value = value; }

        /** \brief The members of the group. */
        std::list<ProFormaMembershipDescriptor> Members() { return _members; }

        /** \brief The value getter and setter. */
        bool IsChanging() { return _isChanging; }

        void AddMember(ProFormaMembershipDescriptor descriptor) { _members.push_back(descriptor);  }
    protected:
        std::string _name;
        ProFormaKey _key;
        ProFormaEvidenceType _evidenceType;
        std::string _value;
        std::list<ProFormaMembershipDescriptor> _members;
        bool _isChanging;
	};
}
