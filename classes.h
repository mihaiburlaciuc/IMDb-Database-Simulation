// Copyright[2017] <Burlaciuc Mihai>
#ifndef _CLASSES_H_
#define _CLASSES_H_
#include <string>
#include <vector>
#include <map>
#include <iomanip> // pentru a seta precizia cu 2 zecimale
#include <sstream> // pentru a putea sa retinem pricizia intr-un string
using std::vector;
using std::string;
using std::map;
using std::iterator;

#define sec_an 31556926

class Movie {
    string movie_name;
    string movie_id;
    int timestamp;
    string director_name;
    vector<string> actor_ids;
    vector<string> categories;
    int rating;
    int user_rating_number;
    map<string, int> user_ratings;
 public:
     explicit Movie(string movie_name, string movie_id, int timestamp,
     vector<std::string> categories, string director_name,
     vector<std::string> actor_ids) {
         this->movie_name = movie_name;
         this->movie_id = movie_id;
         this->timestamp = timestamp;
         this->director_name = director_name;
         this->rating = 0;
         this->user_rating_number = 0;
         unsigned int it;
         // poate e gresit asa
         for (it = 0; it < categories.size(); it++) {
             string a = categories[it];
             this->categories.push_back(a);

         }
         for (it = 0; it < actor_ids.size(); it++) {
             this->actor_ids.push_back(actor_ids[it]);
         }
     }

     ~Movie() {

     }

     Movie(const Movie& obj) {
         this->movie_name = obj.movie_name;
         this->movie_id = obj.movie_id;
         this->timestamp = obj.timestamp;
         this->director_name = obj.director_name;
         this->rating = obj.rating;
         this->user_rating_number = obj.user_rating_number;
         unsigned int it;
         for (it = 0; it < obj.categories.size(); it++) {
             this->categories.push_back(obj.categories[it]);
         }
         for (it = 0; it < obj.actor_ids.size(); it++) {
             this->actor_ids.push_back(obj.actor_ids[it]);
         }
     }

     Movie operator=(const Movie& obj) {
         this->movie_name = obj.movie_name;
         this->movie_id = obj.movie_id;
         this->timestamp = obj.timestamp;
         this->director_name = obj.director_name;
         this->rating = obj.rating;
         this->user_rating_number = obj.user_rating_number;
         unsigned int it;
         for (it = 0; it < obj.categories.size(); it++) {
             this->categories.push_back(obj.categories[it]);
         }
         for (it = 0; it < obj.actor_ids.size(); it++) {
             this->actor_ids.push_back(obj.actor_ids[it]);
         }

         return *this;
     }

     void add_rating(string user_id, int rating) {
         user_ratings.insert(std::pair<string, int>(user_id, rating));
         this->rating += rating;
         user_rating_number++;

     }


     void update_rating(string user_id, int rating) {
         this->rating -= user_ratings.find(user_id)->second;
         user_ratings.at(user_id) = rating;
         this->rating += rating;
     }
     void delete_rating(string user_id) {
         this->rating -= user_ratings.find(user_id)->second;
         user_rating_number--;
         user_ratings.erase(user_id); // trebuie sa stearga user-ul
     }
     string get_movie_id() const{
         return movie_id;
     }
     int get_timestamp() const{
         return timestamp;
     }
     int return_user_rating_number() {
         return user_rating_number;
     }
     double get_double_rating() {
         if (user_rating_number != 0) {
             double average_rating = (double) rating / user_rating_number;
             return average_rating;
         } else {
             return -1;
         }
     }
     string get_rating() {
         if (user_rating_number == 0) {
             return "none";
         }
         double average_rating = (double) rating / user_rating_number;
         std::stringstream stream;
         stream << std::fixed << std::setprecision(2) << average_rating;

         return stream.str();
     }

    void a() {
        unsigned it;
        for (it = 0; it < categories.size(); it++) {
            if (categories[it] == "Adventure") {

                std::cout<<movie_name<<" ";
                std::cout<<timestamp/sec_an + 1970<<" ";
                std::cout<<(double) rating /user_rating_number;
                std::cout<<"\n";
            }
        }
    }

};

class User {
    string user_id;
    string name;
 public:
    explicit User(string user_id, string name) {
        this->user_id = user_id;
        this->name = name;
    }

    ~User() {

    }

    User(const User& obj) {
        this->user_id = obj.user_id;
        this->name = obj.name;
    }

    User operator=(const User& obj) {
        this->user_id = obj.user_id;
        this->name = obj.name;

        return *this;
    }
};

class Actor {
    string actor_id;
    string name;
    int first_time;
    int last_time;
    int id_pt_graf;
 public:
     Actor() {
         this->actor_id = "id";
         this->name = "name";
     }

     explicit Actor(string actor_id, string name, int id_pt_graf) {
         this->actor_id = actor_id;
         this->name = name;
         this->first_time = -1;
         this->last_time = 0;
         this->id_pt_graf = id_pt_graf;
     }

     ~Actor() {

     }

     Actor(const Actor& obj) {
         this->actor_id = obj.actor_id;
         this->name = obj.name;
         this->first_time = obj.first_time;
         this->last_time = obj.last_time;
         this->id_pt_graf = obj.id_pt_graf;
     }

     Actor operator=(const Actor& obj) {
         this->actor_id = obj.actor_id;
         this->name = obj.name;
         this->first_time = obj.first_time;
         this->last_time = obj.last_time;
         this->id_pt_graf = obj.id_pt_graf;
         return *this;
     }

     void add_time(int timestamp) {
         if (first_time == -1) {
             first_time = timestamp;
             last_time = timestamp;
         } else if (timestamp < first_time) {
             first_time = timestamp;
         } else if (timestamp > last_time) {
             last_time = timestamp;
         }
     }
     int get_first_time() {
         return this->first_time;
     }
     int get_career() {
         if (first_time != -1) {
             return (last_time - first_time);
         } else {
             return -1;
         }
     }

     string get_actor_id() {
         return this->actor_id;
     }

     string get_name() {
         return this->name;
     }

     int get_id_graf() {
        return this->id_pt_graf;
     }
};


/*class MovieRating {
    int sum_of_rating;
    int movies_number;

    MovieRating() {
        sum_of_rating = 0;
        movies_number = 0;
    }

    void add_rating(int rating) {
        sum_of_rating += rating;
    }

    double get_avg_rating() {
        return (double) (sum_of_rating / movies_number);
    }
};*/

class MovieNumberRatings {
    string movie_id;
    int number_ratings;
 public:
    MovieNumberRatings() {
        this->movie_id = "";
        this->number_ratings = 0;
    }

    explicit MovieNumberRatings(string movie_id, int number_ratings) {
        this->movie_id = movie_id;
        this->number_ratings = number_ratings;
    }

    ~MovieNumberRatings() {

    }

    void replace(string movie_id, int number_ratings) {
        this->movie_id = movie_id;
        this->number_ratings = number_ratings;
    }



    MovieNumberRatings operator=(const MovieNumberRatings& obj) {
        this->movie_id = obj.movie_id;
        this->number_ratings = obj.number_ratings;
        return *this;
    }

    MovieNumberRatings(const MovieNumberRatings& obj) {
        this->movie_id = obj.movie_id;
        this->number_ratings = obj.number_ratings;
    }
    bool operator==(const MovieNumberRatings& obj) {
        return (number_ratings == obj.number_ratings);
    }

    bool operator<(const MovieNumberRatings& obj) {
        return (number_ratings < obj.number_ratings);
    }

    bool operator>(const MovieNumberRatings& obj) {
        return (number_ratings > obj.number_ratings);
    }

    bool operator!=(const MovieNumberRatings& obj) {
        return (number_ratings != obj.number_ratings);
    }

    bool operator>=(const MovieNumberRatings& obj) {
        return (number_ratings >= obj.number_ratings);
    }

    bool operator<=(const MovieNumberRatings& obj) {
        return (number_ratings <= obj.number_ratings);
    }

    string get_movie() const{
        return movie_id;
    }

    int return_user_rating_number() const{
        return number_ratings;
    }

};

class MovieTime {
    string movie_id;
    int timestamp;
public:
    MovieTime() {
        this->movie_id = "";
        this->timestamp = 0;
    }

    explicit MovieTime(string movie_id, int timestamp) {
        this->movie_id = movie_id;
        this->timestamp = timestamp;
    }

    ~MovieTime() {

    }

    MovieTime operator=(const MovieTime& obj) {
        this->movie_id = obj.movie_id;
        this->timestamp = obj.timestamp;

        return *this;
    }

    MovieTime(const MovieTime& obj) {
        this->movie_id = obj.movie_id;
        this->timestamp = obj.timestamp;
    }
    bool operator==(const MovieTime& obj) {
        return (timestamp == obj.timestamp);
    }

    bool operator<(const MovieTime& obj) {
        return (timestamp < obj.timestamp);
    }

    bool operator>(const MovieTime& obj) {
        return (timestamp > obj.timestamp);
    }

    bool operator!=(const MovieTime& obj) {
        return (timestamp != obj.timestamp);
    }

    bool operator>=(const MovieTime& obj) {
        return (timestamp >= obj.timestamp);
    }

    bool operator<=(const MovieTime& obj) {
        return (timestamp <= obj.timestamp);
    }

    string get_movie() {
        return movie_id;
    }
};
class Colleague {
    string actor_id;
    int movies_number;
public:
    Colleague() {
        this->actor_id = "";
        this->movies_number = 0;
    }

    explicit Colleague(string actor_id, int movies_number) {
        this->actor_id = actor_id;
        this->movies_number = movies_number;
    }

    ~Colleague() {

    }
    int get_movies_number() const{
        return movies_number;
    }
    string get_actor_id() const{
        return actor_id;
    }
    void a() {
        std::cout<< this->actor_id<<" "<<this->movies_number;
    }

    Colleague operator=(const Colleague& obj) {
        this->actor_id = obj.actor_id;
        this->movies_number = obj.movies_number;

        return *this;
    }

    Colleague(const Colleague& obj) {
        this->actor_id = obj.actor_id;
        this->movies_number = obj.movies_number;
    }

    bool operator==(const Colleague& obj) {
        return (movies_number == obj.movies_number && actor_id == obj.actor_id);
    }

    bool operator<(const Colleague& obj) {
        if (movies_number == obj.movies_number) {
            return (actor_id < obj.actor_id);
        }
        return (movies_number > obj.movies_number);
    }

    bool operator>(const Colleague& obj) {
        return (movies_number > obj.movies_number);
    }

    bool operator!=(const Colleague& obj) {
        return (movies_number != obj.movies_number);
    }

    bool operator>=(const Colleague& obj) {
        if (movies_number == obj.movies_number) {
            return actor_id > obj.actor_id;
        }
        return (movies_number >= obj.movies_number);
    }

    bool operator<=(const Colleague& obj) {
        return (movies_number <= obj.movies_number);
    }

    string get_actor_id() {
        return actor_id;
    }
};

#endif // _CLASSES_H_
