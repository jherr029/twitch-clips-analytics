#!/bin/bash
docker run -it --rm -v $(pwd):/app \
        --env-file ./app-env.list \
        cpp-app 