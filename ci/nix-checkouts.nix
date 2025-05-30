let
  byCommit = {
    commit,
    sha256,
  }: import (builtins.fetchTarball {
    name = "nixpkgs-${commit}";
    url = "https://github.com/nixos/nixpkgs/archive/${commit}.tar.gz";
    inherit sha256;
  }) {};
in {
  # For old qt versions, grab the commit before the version bump that has all the patches
  # instead of the bumped version.

  qt6_8_1 = byCommit {
    commit = "3df3c47c19dc90fec35359e89ffb52b34d2b0e94";
    sha256 = "1lhlm7czhwwys5ak6ngb5li6bxddilb9479k9nkss502kw8hwjyz";
  };

  qt6_8_0 = byCommit {
    commit = "352f462ad9d2aa2cde75fdd8f1734e86402a3ff6";
    sha256 = "02zfgkr9fpd6iwfh6dcr3m6fnx61jppm3v081f3brvkqwmmz7zq1";
  };

  qt6_7_3 = byCommit {
    commit = "273673e839189c26130d48993d849a84199523e6";
    sha256 = "0aca369hdxb8j0vx9791anyzy4m65zckx0lriicqhp95kv9q6m7z";
  };

  qt6_7_2 = byCommit {
    commit = "841f166ff96fc2f3ecd1c0cc08072633033d41bf";
    sha256 = "0d7p0cp7zjiadhpa6sdafxvrpw4lnmb1h673w17q615vm1yaasvy";
  };

  qt6_7_1 = byCommit {
    commit = "69bee9866a4e2708b3153fdb61c1425e7857d6b8";
    sha256 = "1an4sha4jsa29dvc4n9mqxbq8jjwg7frl0rhy085g73m7l1yx0lj";
  };

  qt6_7_0 = byCommit {
    commit = "4fbbc17ccf11bc80002b19b31387c9c80276f076";
    sha256 = "09lhgdqlx8j9a7vpdcf8sddlhbzjq0s208spfmxfjdn14fvx8k0j";
  };

  qt6_6_3 = byCommit {
    commit = "8f1a3fbaa92f1d59b09f2d24af6a607b5a280071";
    sha256 = "0322zwxvmg8v2wkm03xpk6mqmmbfjgrhc9prcx0zd36vjl6jmi18";
  };

  qt6_6_2 = byCommit {
    commit = "0bb9cfbd69459488576a0ef3c0e0477bedc3a29e";
    sha256 = "172ww486jm1mczk9id78s32p7ps9m9qgisml286flc8jffb6yad8";
  };

  qt6_6_1 = byCommit {
    commit = "8eecc3342103c38eea666309a7c0d90d403a039a";
    sha256 = "1lakc0immsgrpz3basaysdvd0sx01r0mcbyymx6id12fk0404z5r";
  };

  qt6_6_0 = byCommit {
    commit = "1ded005f95a43953112ffc54b39593ea2f16409f";
    sha256 = "1xvyd3lj81hak9j53mrhdsqx78x5v2ppv8m2s54qa2099anqgm0f";
  };
}
