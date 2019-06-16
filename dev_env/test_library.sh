make build --directory ../  ONIG_INCLUDE_DIR=dev_env/onigmo/include  # CC_USER_DEFINES="-DDEBUG"
make test --directory ../ ONIG_INCLUDE_DIR=dev_env/onigmo/include
