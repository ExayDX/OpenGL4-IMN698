#ifndef __SHADER_MACRO_H_
#define __SHADER_MACRO_H_

#include <string>

typedef struct ShaderMacro
{
	std::string m_name;
	std::string m_value; 

	std::string convertToString()
	{
		std::string macro = "#define ";
		macro.append(m_name);
		macro.append(" ");
		macro.append(m_value);
		macro.append("\n"); 

		return macro;
	}
};

#endif //!__SHADER_MACRO_H_