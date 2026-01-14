

#ifndef OOP_REVIEWSERVICE_H
#define OOP_REVIEWSERVICE_H
#include <string>
#include <ctime>
#include <memory>
#include <vector>

class AppState;
class Review;

class ReviewService {
private:
    static std::vector<Review>& getReviewuriPublicatie(AppState& app, const std::string& id_pub);
public:

    static bool verificaReview(const AppState& app, const std::string& username,const std::string& id_pub);

    static std::vector<const Review *> getReviewuriSortate(const AppState &app, const std::string &id_pub);

    static std::vector<const Review *> getReviewuriNeverificate(const AppState &app, const std::string &id_pub);

    static void stergeReviewuriNeverificate(AppState& app,const std::string& id_pub,const std::vector<int>& indexuri);

    static double calculeazaRatingPublicatieVerificate(const AppState& app,const std::string& identificator_publicatie);

    static double calculeazaRatingPublicatieall(const AppState& app, const std::string& identificator_publicatie);
};
#endif //OOP_REVIEWSERVICE_H