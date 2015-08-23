#include <Refal2.h>

using namespace Refal2;

class CErrorHandler : public IErrorHandler {
public:
	virtual void Error( const std::string& errorText );
	virtual void Warning( const std::string& warningText );
};

void CErrorHandler::Error( const std::string& errorText )
{
	std::cerr << errorText << std::endl;
}
void CErrorHandler::Warning( const std::string& warningText )
{
	std::cerr << warningText << std::endl;
}

bool ParseFile( CScanner& scanner, const std::string& fileName )
{
	std::ifstream fileStream( fileName );
	if( !fileStream.good() ) {
		std::cerr << "Cannot open file: `" << fileName << "`." << std::endl;
		std::cerr << "------------------------" << std::endl << std::endl;
		return false;
	}
	std::cout << "Source file: `" << fileName << "`." << std::endl;

	for( char c; fileStream.get( c ); ) {
		scanner.AddChar( c );
	}
	scanner.AddEndOfFile();

	std::cout << "------------------------" << std::endl << std::endl;
	std::cerr << "------------------------" << std::endl << std::endl;

	return ( !scanner.HasErrors() );
}

typedef std::vector<std::string> CFileNames;

CProgramPtr ParseFiles( const CFileNames& fileNames )
{
	CErrorHandler errorHandler;
	CScanner scanner( &errorHandler );

	bool success = true;
	for( CFileNames::const_iterator i = fileNames.begin();
		i != fileNames.end(); ++i )
	{
		success &= ParseFile( scanner, *i );
	}

	if( success ) {
		return scanner.BuildProgram();
	}
	return CProgramPtr();
}

const char* const ExecutionResultStrings[] = {
	"ER_OK",
	"ER_RecognitionImpossible",
	"ER_CallEmptyFunction",
	"ER_LostFunctionLabel",
	"ER_WrongArgumentOfExternalFunction"
};

int main( int argc, const char* argv[] )
{
	std::ios::sync_with_stdio( false );
	if( argc < 2 ) {
		std::cerr << "Wrong arguments: source files were not set." << std::endl;
		return 1;
	}

	CFileNames fileNames;
	for( int i = 1; i < argc; i++ ) {
		fileNames.push_back( argv[i] );
	}

	CProgramPtr program = ParseFiles( fileNames );

	if( !static_cast<bool>( program ) ) {
		return 1;
	}

	CUnitList fieldOfView;
	CUnitNode* errorNode;
	TExecutionResult result =
		COperationsExecuter::Run( program, fieldOfView, errorNode );

	std::cout << ExecutionResultStrings[result] << std::endl;

	return 0;
}
