# hallServer
https://hub.docker.com/repository/docker/alinagazaryan/exam_25_06_2023_test/general

# command build docker
sudo chown -R $(whoami) ~/.docker

docker build .

docker run --rm -p 33333:33333 --name exam_25_06_2023_test alinagazaryan/exam_25_06_2023_test
