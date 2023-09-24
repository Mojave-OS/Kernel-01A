# Mojave Development Environment

When developing code for the ARM AArch64 Platform, you have to have the proper compiler(s). I've gone ahead and made a docker environment that sets that up for you! It's not the cleanest, but it'll work for now. 

## How to use?

Using this is a pretty easy process. First, go ahead and make sure that you've got Docker installed on your machine.

> [Docker Installation Guide](https://docs.docker.com/desktop/)

**Once you have docker installed** and are in the Kernel-01A directory:

1. `cd tools/aarch64-linux-gnu`: This will put you in a directory that has a `Dockerfile`, `build-script.sh` file, and a `start.sh` script. 
2. `[sudo] docker build -t aarch64-cross-img .`: This may require a sudo user if your user is not part of the docker group. Try without sudo, if it fails then run it with sudo. This will build a docker image using the Dockerfile I've provided. *This may take a while*.
3. `sh start.sh`: This will start a docker container with the docker image that you just built. In this environment, you'll be able to compile programs for the mojave OS. 