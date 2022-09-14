# JODATUNI 2022 project

This is exercise project for the course "Johdatus datatieteeseen 2022". The project utilizes data from <a href="http://insideairbnb.com/" target="_blank">Inside Airbnb</a> and <a href="https://data.london.gov.uk/dataset/cultural-infrastructure-map">Cultural Infrastructure Map of London</a>. The aim of the project is to analyze how different variables impact the price of Airbnb apartments in the London area. The factors researched are variables considering the apartment itself and its qualities, as well as the distances to some clutural infrastructure locations, which are pubs, cinemas, musicvenues and museums.

## Running the project

The project consists of the six Python notebooks, each of which describes and implements a part of the project. Any extra settings files are not required to execute the project. The project carries out in the files in the following order:
- Kehitysymparisto.ipynb
- Datan_keraaminen.ipynb
- Jalostaminen.ipynb
- Kuvailu.ipynb
- Koneoppiminen.ipynb
- Toimeenpano.ipynb

To avoid issues with dependencies, it is recommendable to install some type of Anaconda virtual environment, Miniconda for example should work fine. One way to do this is as follows:

1. Install Miniconda, the install script will prompt to start the environment for you
`````
wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh
rm Miniconda3-latest-Linux-x86_64.sh
source .bashrc
`````
2. Install necessary packages
`````
conda install scikit-learn pandas jupyterlab ipython
`````
3. Clone this repo / unzip project files
4. Run jupyter-lab in the project folder
`````
cd PROJECTFOLDER
jupyter-lab
`````

The first file in the previously provided list mainly describes the environment in which I developed the project, and the project itself could be said to start from the second file which describes the process of collecting data to work on. The third file describes the process which I took to preprocess the data, such as cleaning and modifying the dimensions etc. The fourth file is in a way expoloratory analysis, where I visualize the data in different ways. The fifth file is where I analyze the data using machine learning techniques, and in the sixth file I describe the implications of the results to the end user.
