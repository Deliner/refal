#include <iostream>
#include <Refal2.h>

namespace Refal2 {

void HandyPrintFieldOfView( const CUnitList& fieldOfView )
{
	bool lastWasChar = false;
	for( const CUnitNode* i = fieldOfView.GetFirst(); i != 0; i = i->Next() ) {
		if( i->GetType() == UT_Char ) {
			if( !lastWasChar ) {
				std::cout << "'";
				lastWasChar = true;
			}
		} else {
			if( lastWasChar ) {
				std::cout << "'";
				lastWasChar = false;
			}
		}
		switch( i->GetType() ) {
			case UT_Char:
				std::cout << i->Char();
				break;
			case UT_Label:
				std::cout << "/" << LabelTable.GetLabelText( i->Label() ) << "/";
				break;
			case UT_Number:
				std::cout << "/" << i->Number() << "/";
				break;
			case UT_LeftParen:
				std::cout << "(";
				break;
			case UT_RightParen:
				std::cout << ")";
				break;
			case UT_LeftBracket:
				std::cout << "<";
				break;
			case UT_RightBracket:
				std::cout << ">";
				break;
			default:
				assert( false );
				break;
		}
	}
	if( lastWasChar ) {
		std::cout << "'";
	}
	std::cout << std::endl;
}

void PrintUnit(const CUnit& unit, const CVariables* variables)
{
	switch( unit.GetType() ) {
		case UT_Char:
			std::cout << "'" << unit.Char() << "' ";
			break;
		case UT_Label:
			std::cout << "/" << LabelTable.GetLabelText( unit.Label() ) << "/ ";
			break;
		case UT_Number:
			std::cout << "/" << unit.Number() << "/ ";
			break;
		case UT_Variable:
			if( variables != 0 ) {
				std::cout <<
					variables->GetVariable( unit.Variable() )->GetType();
				PrintQualifier(
					variables->GetVariable( unit.Variable() )->GetQualifier() );
			} else {
				std::cout << "V";
			}
			std::cout << unit.Variable() << " ";
			break;
		case UT_LeftParen:
			std::cout << "( ";
			break;
		case UT_RightParen:
			std::cout << ") ";
			break;
		case UT_LeftBracket:
			std::cout << "< ";
			break;
		case UT_RightBracket:
			std::cout << "> ";
			break;
		default:
			assert( false );
			break;
	}
}

void PrintUnitList(const CUnitNode* fromNode, const CUnitNode* toNode,
	const CVariables* variables)
{
	if( fromNode == 0 || toNode == 0 ) {
		return;
	}
	
	for( ; fromNode != toNode; fromNode = fromNode->Next() ) {
		PrintUnit( *fromNode, variables );
	}
	PrintUnit( *fromNode, variables );
}

bool CompareUnit(const CUnit& unitA, const CUnit& unitB)
{
	if( unitA.type != unitB.type ) {
		return false;
	}

	switch( unitA.type ) {
		case UT_Char:
			return ( unitA.c == unitB.c );
			break;
		case UT_Label:
			return ( unitA.label == unitB.label );
			break;
		case UT_Number:
			return ( unitA.number == unitB.number );
			break;
		case UT_Variable:
			return ( unitA.variable == unitB.variable );
			break;
		case UT_LeftParen:
		case UT_RightParen:
		case UT_LeftBracket:
		case UT_RightBracket:
			return true;
			break;
		default:
			break;
	}
	assert( false );
	return false;
}

} // end of namespace Refal2
