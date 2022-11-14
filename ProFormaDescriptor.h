#pragma once

#include <ostream>

#include "IProFormaDescriptor.h"

namespace ProForma {
	/**
	 * \class ProFormaDescriptor
	 *
	 * \brief Member of the tag. Could be a key-value pair, or a keyless entry.
	 *
	 */

	class ProFormaDescriptor : public IProFormaDescriptor {
	public:
        /** \brief  Initializes a descriptor with value only
		  * \param  value Value to be assigned to the descriptor, use defaults for the rest
		  * \return void
		  */
        ProFormaDescriptor(std::string value)
            : ProFormaDescriptor(ProFormaKey::Name, ProFormaEvidenceType::None, value) { }

        /** \brief  Initializes a descriptor with key and value
          * \param  key Key to be assigned to the descriptor
		  * \param  value Value to be assigned to the descriptor
		  * \return void
		  */
        ProFormaDescriptor(ProFormaKey key, std::string value)
            : ProFormaDescriptor(key, ProFormaEvidenceType::None, value) { }

        /** \brief  Initializes a descriptor with all parameters
          * \param  key Key to be assigned to the descriptor
          * \param  evidenceType Value to be assigned to the evidenceType
		  * \param  value Value to be assigned to the descriptor
		  * \return void
		  */
        ProFormaDescriptor(ProFormaKey key, ProFormaEvidenceType evidenceType, std::string value)
        {
            this->_key = key;
            this->_evidenceType = evidenceType;
            this->_value = value;
        }

        /** \brief  Key getter */
        ProFormaKey Key() { return _key; }

        /** \brief The type of the evidence getter */
        ProFormaEvidenceType EvidenceType() { return _evidenceType; }

        /** \brief The value getter. */
        std::string Value() { return _value; }

        /** \brief Returns the string representation for descriptor object */
        std::string ToString() const
        {
            auto key = std::to_string(static_cast<std::underlying_type<ProFormaKey>::type>(_key));
            auto evidenceType = std::to_string(static_cast<std::underlying_type<ProFormaEvidenceType>::type>(_evidenceType));

            return std::string(key + ":" + evidenceType + ":" + _value);
        }

        /// <summary>String representation of <see cref="ProFormaDescriptor"/></summary>
        /// <returns></returns>
        friend std::ostream & operator<<(std::ostream& stream, ProFormaDescriptor const& descriptor) {
            stream << descriptor.ToString();
        }
    protected:
        ProFormaKey  _key;
        ProFormaEvidenceType _evidenceType;
        std::string _value;
	};
}
