#include "ScoreManager.h"

ScoreManager* ScoreManager::instance = NULL;

ScoreManager::ScoreManager()
{
	resetScore();
}

ScoreManager* ScoreManager::sharedScoreManager()
{
	if (instance == NULL)
	{
		instance = new ScoreManager();
	}

	return instance;
}

void ScoreManager::resetScore()
{
	// TODO
}

void ScoreManager::compareScoreToHighscore()
{
	// TODO
}

void ScoreManager::storeHighscoreToFile(int level, int star, std::string time)
{
	bool levelTagFound = false;
	bool starTagFound = false;
	bool timeTagFound = false;

	// Insert code to save highscore to file
	std::string path = getFilePath();
	std::string fileContents = "";

	std::ifstream infile(path);
	std::string currLine;

	if (infile.good()) {
		// Keep looping through the file to find the level tag
		while (std::getline(infile, currLine)) {
			if (currLine.find("<level_" + StringUtils::format("%d", level) + ">") == true) {
				levelTagFound = true;
			}
			fileContents += currLine + "\n";
		}

		if (!levelTagFound) {
			int indexTagEnd = fileContents.find("</data>");
			std::string insertString = "\t<level_" + StringUtils::format("%d", level) + ">\n";
			insertString += "\t\t<star>" + StringUtils::format("%d", star) + "</star>\n";
			insertString += "\t\t<time>" + time + "</time>\n";
			insertString += "\t</level_" + StringUtils::format("%d", level) + ">\n";

			std::string firstHalf = "";
			std::string secondHalf = "";

			for (int i = 0; i < indexTagEnd; i++) {
				firstHalf += fileContents[i];
			}
			for (int i2 = indexTagEnd; i2 < fileContents.size(); i2++) {
				secondHalf += fileContents[i2];
			}

			fileContents = firstHalf + insertString + secondHalf;
		}
		else {
			int indexTagStart = fileContents.find("<level_" + StringUtils::format("%d", level) + ">");
			int indexTagEnd = fileContents.find("</level_" + StringUtils::format("%d", level) + ">");
						
			std::string firstHalf = "";
			std::string secondHalf = "";

			std::string levelTagString = "<level_" + StringUtils::format("%d", level) + ">";

			// Add entire document string up to the end of the level tag
			for (int i = 0; i < (indexTagStart + levelTagString.length()); i++) {
				firstHalf += fileContents[i];
			}

			fileContents = firstHalf;

			// Now add the other tags
			fileContents += "\n\t\t<star>" + StringUtils::format("%d", star) + "</star>";
			fileContents += "\n\t\t<time>" + time + "</time>";
			fileContents += "\n\t";

			for (int i2 = indexTagEnd; i2 < fileContents.size(); i2++) {
				secondHalf += fileContents[i2];
			}

			fileContents += secondHalf;
		}

		std::fstream output;
		output.open(path);

		output << fileContents;

		output.close();
	}
	else {
		FILE *fp = fopen(path.c_str(), "w");

		if (!fp) {
			CCLOG("Cannot create file in %s", path.c_str());
			return;
		}

		std::string value = StringUtils::format("<?xml version=\"1.0\" encoding=\"UTF - 8\"?>");
		value += StringUtils::format("\n<!DOCTYPE SB76-PBS-ABHS PUBLIC>");
		value += StringUtils::format("\n<data>");
		value += StringUtils::format("\n\t<level_%d>", level);
		value += StringUtils::format("\n\t\t<star>%d</star>", star);
		value += StringUtils::format("\n\t\t<time>") + time + StringUtils::format("</time>");
		value += StringUtils::format("\n\t</level_%d>", level);
		value += StringUtils::format("\n</data>");

		fputs(value.c_str(), fp);
		fclose(fp);
	}
}

int ScoreManager::getStarFromFile(int level)
{
	bool levelTagFound = false;
	bool starTagFound = false;

	int starRating = 0;
	//std::string tempHighScore = "";

	// Insert code to get highscore from file
	std::string path = getFilePath();

	std::ifstream infile(path);
	std::string currLine;

	while (std::getline(infile, currLine)) {
		if (!starTagFound) {
			if (!levelTagFound) {
				int found = currLine.find("<level_" + StringUtils::format("%d", level) + ">");
				if (found != -1) {
					// Level tag has been found
					levelTagFound = true;
				}
			}
			// Check if level tag has been found, otherwise these checks are not worth doing
			else {
				int found = currLine.find("<star>");

				if (found != -1) {
					// Star tag has been found
					starTagFound = true;

					int indexScoreStart = currLine.find(">");
					int indexScoreEnd = currLine.find("<", indexScoreStart + 1);

					starRating = currLine.at(indexScoreStart + 1);
				}
			}
		}
	}

	return starRating;
}

std::string ScoreManager::getTimeFromFile(int level)
{
	bool levelTagFound = false;
	bool timeTagFound = false;

	//int starRating = 0;
	std::string time = "No Recorded Time";

	// Insert code to get highscore from file
	std::string path = getFilePath();

	std::ifstream infile(path);
	std::string currLine;

	while (std::getline(infile, currLine)) {
		if (!timeTagFound) {
			if (!levelTagFound) {
				int found = currLine.find("<level_" + StringUtils::format("%d", level) + ">");
				if (found != -1) {
					// Level tag has been found
					levelTagFound = true;
				}
			}
			// Check if level tag has been found, otherwise these checks are not worth doing
			else {
				int found = currLine.find("<time>");

				if (found != -1) {
					// Time tag has been found
					timeTagFound = true;

					int indexScoreStart = currLine.find(">");
					int indexScoreEnd = currLine.find("<", indexScoreStart + 1);

					time = "";

					for (int i = indexScoreStart + 1; i < indexScoreEnd; i++) {
						time += currLine.at(i);
					}
				}
			}
		}
	}

	return time;
}

std::string ScoreManager::getFilePath()
{
	std::string path = "";

	// testing
	std::string writableDir = CCFileUtils::getInstance()->getWritablePath();

	path = writableDir + "\\highscoredata.xml";

	return path;
}

ScoreManager::~ScoreManager()
{
}