//function to generate random number
Request generateRandomRequest(int platterCount, int cylinderCount, int sectorCount){
    Request newRequest;
    newRequest.cylinder = generateRandomInteger(0, cylinderCount - 1);
    newRequest.platter = generateRandomInteger(0,  platterCount - 1);
    newRequest.sector = generateRandomInteger(0, sectorCount - 1);
    newRequest.sector_count = generateRandomInteger(1, sectorCount);
    return newRequest;
}
