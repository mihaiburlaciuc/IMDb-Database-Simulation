#include <iterator>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "include/imdb.h"
using std::string;
using std::cout;

#define sec_an 31556926
IMDb::IMDb() {
    // initialize what you need here.

}

IMDb::~IMDb() {

}

void IMDb::add_movie(std::string movie_name,
                     std::string movie_id,
                     int timestamp,  // unix timestamp when movie was launched
                     std::vector<std::string> categories,
                     std::string director_name,
                     std::vector<std::string> actor_ids) {
    Movie tempMovie(movie_name, movie_id, timestamp, categories, director_name,
                    actor_ids);
    this->movies.insert(std::pair<std::string, Movie> (movie_id, tempMovie));
    // Necesar pentru determinarea functiei get_best_year_for_category
    int year = timestamp / sec_an + 1970;
    // Folosim un map ce se foloseste de categorii pentru a pointa catre
    // un map ce are toti anii, in care se afla toate filmele din acel an
    for (unsigned int i = 0; i < categories.size(); i++) {

        if (categoriesMap.find(categories[i]) != categoriesMap.end()) {
            std::map<int, std::vector<string> > tempYearMap;
            tempYearMap = categoriesMap.find(categories[i])->second;
            if (tempYearMap.find(year) == tempYearMap.end()) {
                std::vector<string> tempMovieVector;
                tempMovieVector.push_back(movie_id);
                tempYearMap.insert(std::pair<int, std::vector<string> > (year, tempMovieVector));
            } else {
                std::vector<string> thisMovieVector = tempYearMap.find(year)->second;
                thisMovieVector.push_back(movie_id);
                tempYearMap.at(year) = thisMovieVector;
            }
            categoriesMap.at(categories[i]) = tempYearMap;
        } else {
            std::map<int, std::vector<string> > tempYearMap;
            std::vector<string> tempMovieVector;
            tempMovieVector.push_back(movie_id);
            tempYearMap.insert(std::pair<int, std::vector<string> > (year, tempMovieVector));
            categoriesMap.insert(std::pair<string, std::map<int, std::vector<string> > >
            (categories[i], tempYearMap));
        }
    }
    // Add_movie
    this->movies.insert(std::pair<std::string, Movie> (movie_id, tempMovie));

    // pentru get_2nd_degree_colleagues
    for (unsigned int i = 0; i < actor_ids.size(); i++) {
        int id_graf_actor = actors.at(actor_ids[i]).get_id_graf();
        // actorii si colaboratorii sigur exista
        for (unsigned int j = i+1; j < actor_ids.size(); j++) {
            int id_graf_colab = actors.at(actor_ids[j]).get_id_graf();
            if (id_graf_actor != id_graf_colab) {
                this->graph_for_actors.add_edge(id_graf_actor, id_graf_colab);
            }
        }
    }
    // Necesar pentru determinarea functiei get_longest_career_actor
    // Adaugare timestamp
    for (unsigned int it = 0; it < actor_ids.size(); it++) {
        actors.at(actor_ids[it]).add_time(timestamp);
    }
    // Necesar pentru determinarea functiei get_most_influential_director
    std::map<std::string, std::vector<std::string> >::iterator it;
    it = directors.find(director_name);
  	if (it == directors.end()) {  // daca acest director nu a mai fost adaugat
  		this->directors.insert(std::pair<std::string,
            std::vector<std::string> > (director_name, actor_ids));
  	} else {
  		for (unsigned int i = 0; i < actor_ids.size(); i++) {
  			std::vector<std::string>::iterator it2;
  			// verificam daca directorul a mai jucat cu acesti actori
  			it2 = find(directors.at(director_name).begin(),
  				directors.at(director_name).end(), actor_ids[i]);
  			if (it2 == directors.at(director_name).end()) {
  				// daca nu exista actorul il adaugam in vectorul directorului
  				directors.at(director_name).push_back(actor_ids[i]);
  			}
  		}
  	}

    // Necesar pt functia get_top_k_most_recent_movies
    MovieTime tempMovieTime(movie_id, timestamp);
    timestampForMovies.insert(tempMovieTime);

    // Necesar pt functia get_top_k_most_popular_movies
    MovieNumberRatings tempMovieRating(movie_id, 0);  // nu a primit niciun vot pana acum;
    popularMovie.insert(tempMovieRating);
//std::cout << movie_name << '\n';
//graph_for_actors.print();
    //Necesar pt functia get_top_k_partners_for_actor
    // for (unsigned int it = 0; it < actor_ids.size(); it++) {
    //         int index1 = actors.at(actor_ids[it]).get_id_graf();
    //
    // }

    // Necesar pt functia get_top_k_actor_pairs


}

void IMDb::add_user(std::string user_id, std::string name) {
    User tempUser(user_id, name);
    this->users.insert(std::pair<string, User> (user_id, tempUser));
}

void IMDb::add_actor(std::string actor_id, std::string name) {
    int size = actors.size();  // id-ul unic pt graf
    Actor tempActor(actor_id, name, size);
    this->actors.insert(std::pair<string, Actor> (actor_id, tempActor));
    graph_for_actors.add_node(size);
    mapForIdGraph.insert(std::pair<int, string> (size, actor_id));
    // Heap<Colleague> tempHeap;
    // actorsHeapMap.insert(std::pair<string, Heap<Colleague> >
    // (actor_id, tempHeap));
}

void IMDb::add_rating(std::string user_id, std::string movie_id, int rating) {
	int oldNumRating = movies.at(movie_id).return_user_rating_number();
    movies.at(movie_id).add_rating(user_id, rating);
    MovieNumberRatings tempMovieRatingOld(movie_id,oldNumRating);
    MovieNumberRatings tempMovieRatingNew(movie_id,oldNumRating+1);
    popularMovie.replace(tempMovieRatingOld, tempMovieRatingNew);
}

void IMDb::update_rating(std::string user_id, std::string movie_id, int rating) {
	int oldNumRating = movies.at(movie_id).return_user_rating_number();
    movies.at(movie_id).update_rating(user_id, rating);
    MovieNumberRatings tempMovieRatingOld(movie_id,oldNumRating);
    MovieNumberRatings tempMovieRatingNew(movie_id,oldNumRating + 1);
    popularMovie.replace(tempMovieRatingOld, tempMovieRatingNew);
 }

void IMDb::remove_rating(std::string user_id, std::string movie_id) {
	int oldNumRating = movies.at(movie_id).return_user_rating_number();
    movies.at(movie_id).delete_rating(user_id);
    MovieNumberRatings tempMovieRatingOld(movie_id,oldNumRating);
    MovieNumberRatings tempMovieRatingNew(movie_id,oldNumRating - 1);
    popularMovie.replace(tempMovieRatingOld, tempMovieRatingNew);
}

std::string IMDb::get_rating(std::string movie_id) {
    string rating = this->movies.at(movie_id).get_rating();
    return rating;
}

std::string IMDb::get_longest_career_actor() {
    int maxim = -1;
    string career_actor_id = "";
    std::map<std::string, Actor>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++) {
        if (it->second.get_career() > maxim) {
            maxim = it->second.get_career();
            career_actor_id = it->first;
        }
    }
    if (maxim == -1) return "none";
    return career_actor_id;
}

std::string IMDb::get_most_influential_director() {
	unsigned int maxim = 0;
	string best_director = "";
	std::map<std::string, std::vector<std::string> >::iterator it;
	for (it = directors.begin(); it != directors.end(); it++) {
		if (it->second.size() > maxim) {
			maxim = it->second.size();
			best_director = it->first;
		}
	}
    if (maxim == 0) {
        return "none";
    }
    return best_director;
}

std::string IMDb::get_best_year_for_category(std::string category) {
    std::map<int, std::vector<string> > tempYearMap;
    if (categoriesMap.find(category) != categoriesMap.end()) {
        tempYearMap = categoriesMap.find(category)->second;
        std::map<int, std::vector<string> >::iterator it;
        double max_rating = 0;
        int best_year = 0;
        int ok = 0;

        for (it = tempYearMap.begin(); it != tempYearMap.end(); it++) {
            std::vector<string> tempVector;
            tempVector = it->second;
            double sum_of_rating = 0;
            int moviesNum = 0;
            for (unsigned int i = 0; i < tempVector.size(); i++) {

                double thisRating = movies.at(tempVector[i]).get_double_rating();
                if (thisRating != -1) {
                	moviesNum++;
                    sum_of_rating += thisRating;
                }
            }
            double ratingYear = -1;
            if (moviesNum != 0)
            ratingYear = (double) sum_of_rating / moviesNum;

            if (ratingYear == 0 && ok == 0 && max_rating == 0) {
            	best_year = it->first;
            	ok = 1;
            }
            if (ratingYear > max_rating) {
                max_rating = ratingYear;
                best_year = it->first;
            }
        }
        string answer;
        if (best_year != 0) {
            answer = std::to_string(best_year);
        } else {
            answer = "none";
        }
        return answer;
    } else {
        return "none";
    }

}

std::string IMDb::get_2nd_degree_colleagues(std::string actor_id) {
    int id_graf_actor = actors.at(actor_id).get_id_graf();
    std::vector<int> a = graph_for_actors.bfs(id_graf_actor);
    std::vector<string> id_vecini;
    for (unsigned int i = 0; i < a.size(); i++) {
          id_vecini.push_back(mapForIdGraph.at(a[i]));
    }
    std::sort (id_vecini.begin(), id_vecini.end());

    string toti_vecinii="";
    for (unsigned int i = 0; i < id_vecini.size() - 1 && a.size() != 0; i++) {
      toti_vecinii += id_vecini[i];
      toti_vecinii += " ";
    }
    if (id_vecini.size() != 0) {
    toti_vecinii += id_vecini[id_vecini.size() - 1];
    }

    if (a.size() == 0) {
        return std::string("none");
    }
    else {
        return toti_vecinii;
    }
}

std::string IMDb::get_top_k_most_recent_movies(int k) {
    string recentMovies = "none";
    std::vector<MovieTime> saver;
    if (!timestampForMovies.isEmpty() && k > 0) {
        recentMovies = "";
        MovieTime tempMovieTime = timestampForMovies.extractMax();
        saver.push_back(tempMovieTime);
        recentMovies += tempMovieTime.get_movie();
    }
    for (int i = 1; i < k && !timestampForMovies.isEmpty(); i++) {
        MovieTime tempMovieTime = timestampForMovies.extractMax();
        saver.push_back(tempMovieTime);
        recentMovies += " ";
        recentMovies += tempMovieTime.get_movie();
    }
    for (unsigned int i = 0; i < saver.size(); i++) {
        timestampForMovies.insert(saver[i]);
    }

    return recentMovies;
    }

std::string IMDb::get_top_k_actor_pairs(int k) {
    std::vector<ActorPairs> tempVector;
    tempVector = graph_for_actors.return_all_edges(this->mapForIdGraph);
    std::sort(tempVector.begin(), tempVector.end());
    string answer;
    if (tempVector.size() != 0) {
        answer = "";
        answer += "(";
        answer += tempVector[0].get_actor1_id();
        answer += " ";
        answer += tempVector[0].get_actor2_id();
        answer += " ";
        answer+= std::to_string(tempVector[0].get_movies_number());
        answer += ")";
        for (unsigned int i = 1; i < k && i < tempVector.size(); i++) {
            answer += " ";
            answer += "(";
            answer += tempVector[i].get_actor1_id();
            answer += " ";
            answer += tempVector[i].get_actor2_id();
            answer += " ";
            answer+= std::to_string(tempVector[i].get_movies_number());
            answer += ")";
        }
        return answer;
    }
    return "none";
}
bool comparatieColleague(const Colleague& i,const Colleague& j) {
	if (i.get_movies_number() == j.get_movies_number())
		return i.get_actor_id() < j.get_actor_id();
	return i.get_movies_number() > j.get_movies_number();
}
std::string IMDb::get_top_k_partners_for_actor(int k, std::string actor_id) {
    int index = actors.at(actor_id).get_id_graf();
    std::vector<Colleague> colleagueVector = graph_for_actors.return_colleagues(index, mapForIdGraph);
    std::sort(colleagueVector.begin(), colleagueVector.end());
    string answer;
    if (colleagueVector.size() != 0) {
        answer = "";
        answer += colleagueVector[0].get_actor_id();
        for (unsigned int i = 1; i < k && i < colleagueVector.size(); i++) {
            answer += " ";
            answer += colleagueVector[i].get_actor_id();
        }
        return answer;
    }
    return "none";


}
bool comparatie(const MovieNumberRatings& i,const MovieNumberRatings& j) {
	if (i.return_user_rating_number() == j.return_user_rating_number())
		return i.get_movie() < j.get_movie();
	return i.return_user_rating_number() > j.return_user_rating_number();
}

std::string IMDb::get_top_k_most_popular_movies(int k) {
	string popular_movies = "none";
    std::vector<MovieNumberRatings> saver, ordonat;
    //popularMovie.print();
    if (!popularMovie.isEmpty() && k > 0) {
        //popular_movies = "";
        MovieNumberRatings tempMovieRating = popularMovie.extractMax();
        saver.push_back(tempMovieRating);
        ordonat.push_back(tempMovieRating);
        //std::cout<<tempMovieRating.get_movie()<<"="<<tempMovieRating.return_user_rating_number()<<"\n";
        //popular_movies += tempMovieRating.get_movie();
    }
    for (int i = 1; i < k && !popularMovie.isEmpty(); i++) {
        MovieNumberRatings tempMovieRating = popularMovie.extractMax();
        saver.push_back(tempMovieRating);
        ordonat.push_back(tempMovieRating);
        //std::cout<<tempMovieRating.get_movie()<<"="<<tempMovieRating.return_user_rating_number()<<"\n";
        //popular_movies += " ";
        //popular_movies += tempMovieRating.get_movie();
    }

    std::sort (ordonat.begin(), ordonat.end(),comparatie);
    //popularMovie.print();
    if (saver.size() != 0) popular_movies = "";
    for (unsigned int i = 0; i < ordonat.size() - 1; i++) {
    	popular_movies += ordonat[i].get_movie();
    	popular_movies += " ";
    }
    popular_movies += ordonat[ordonat.size() - 1].get_movie();
    for (unsigned int i = 0; i < saver.size(); i++) {
        popularMovie.insert(saver[i]);
    }
    //std::cout<<popular_movies<<"-";
    return popular_movies;
}
std::string IMDb::get_avg_rating_in_range(int start, int end) {
    double sum = 0;
    int movies_number = 0;
    std::vector<Movie> movieVector;
    std::map<string, Movie>::iterator it;
    for (it = movies.begin(); it != movies.end(); it++) {
        if (it->second.get_timestamp() >= start
        && it->second.get_timestamp() <= end) {
            if (it->second.return_user_rating_number() != 0) {
                sum += it->second.get_double_rating();
                movies_number++;
            }
        }
    }
    if (movies_number != 0) {
        double average_rating = (double) sum / movies_number;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << average_rating;
        return stream.str();
    } else {
        return "none";
    }
    return "";
}
