/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileTreatment.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:59:02 by tisantos          #+#    #+#             */
/*   Updated: 2022/03/19 16:57:40 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	    /****************/
        /* Constructors */
        /****************/
		configFileTreatment();
		configFileTreatment(const configFileTreatment& rhs);
		configFileTreatment(std::string configuration_file_raw);
		~configFileTreatment();

        /*****************/
        /* Copy Operator */
        /*****************/
        configFileTreatment& operator=(const configFileTreatment& rhs);

        /***********/
        /* Methods */
        /***********/
		void	treatConfigurationFile();
		void	separateToServerBlocks();

		/***********/
        /* Getters */
        /***********/
		std::list<std::vector<std::string> >	getServerBlocks();

		/*************/
        /* Debuggers */
        /*************/
		void    printConfigurationFileTreated();
		void	printServerBlocks();

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
