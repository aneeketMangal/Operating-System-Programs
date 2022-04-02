struct RequestBody generateRandomRequest(int m, int t){
    struct RequestBody newRequest;
    newRequest.size = generateRandom(0.5*m, 3*m, 10);
    newRequest.duration = generateRandom(0.5*t, 6*t, 5);
    newRequest.valid = 1;
    newRequest.time_s = clock();
    return newRequest;
}