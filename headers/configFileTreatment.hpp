#ifndef CONFIGFILETREATMENT_HPP
# define CONFIGFILETREATMENT_HPP

# include <iostream>
# include <vector>
# include <list>
# include <fstream>
# include <sstream>

class configFileTreatment
{
	public:
		configFileTreatment();
		configFileTreatment(std::string configuration_file_raw);
		~configFileTreatment();

		void	treatConfigurationFile();
		void	separateToServerBlocks();
        void    printConfigurationFileTreated();
		void	printServerBlocks();
		std::list<std::vector<std::string> >	getServerBlocks();

	private:
        /********************/
        /* Helper Functions */
        /********************/
		void	treatConfigurationFile_StepOne();
		void	treatConfigurationFile_StepTwo();
		void	treatConfigurationFile_StepThree();
		void	treatConfigurationFile_StepFour();

        /********/
        /* Data */
        /********/
        std::string					_configuration_file_raw;
		std::vector<std::string>	_configuration_file_treated; // Ready to be separated in
																 // server blocks.

        std::list<std::vector<std::string> >	_server_blocks; // This list contains the
																// server blocks.
};

#endif
