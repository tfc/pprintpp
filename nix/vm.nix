let
  fedora30x86_64 = pkgs:
    let
      version = "30";
      commonFedoraPackages = [
        "annobin"
        "autoconf"
        "automake"
        "basesystem"
        "bzip2"
        "curl"
        "diffutils"
        #"fedora-release"
        "findutils"
        "gawk"
        "gcc-c++"
        "gzip"
        "make"
        "patch"
        "perl"
        "pkgconf-pkg-config"
        "rpm"
        "rpm-build"
        "tar"
        "unzip"
      ];
    in rec {
      name = "fedora-${version}-x86_64";
      fullName = "Fedora ${version} (x86_64)";
      packagesList = pkgs.fetchurl rec {
        url = "mirror://fedora/linux/releases/${version}/Everything/x86_64/os/repodata/${sha256}-primary.xml.gz";
        sha256 = "54ed9948b74b314b877fa90468e7c5309a0f9428c35b938b82fa7b356a799c94";
      };
      urlPrefix = "mirror://fedora/linux/releases/${version}/Everything/x86_64/os";
      archs = ["noarch" "x86_64"];
      packages = commonFedoraPackages ++ [ "cronie" "util-linux" "cmake" ];
      unifiedSystemDir = true;
    };
  ubuntu1904x86_64 = pkgs: {
    name = "ubuntu-19.04-disco-amd64";
    fullName = "Ubuntu 19.04 Disco Dingo (amd64)";
    packagesLists =
      [ (pkgs.fetchurl {
          url = mirror://ubuntu/dists/disco/main/binary-amd64/Packages.xz;
          sha256 = "1jxz5jkdwypwr52draand3iiqqkpjcb4g33ykgsr5vp7gc70md22";
        })
        (pkgs.fetchurl {
          url = mirror://ubuntu/dists/disco/universe/binary-amd64/Packages.xz;
          sha256 = "16gnh9ibc7q1y4yxvac2jf9xr7s756yngihx5z2nxnlgxhynr9yy";
        })
      ];
    urlPrefix = mirror://ubuntu;
    packages = pkgs.vmTools.commonDebPackages ++ [ "diffutils" "libc-bin" "cmake" ];
  };
in self: super: {
  vmTools = super.vmTools // rec {
    debDistros = super.vmTools.debDistros // { ubuntu1904x86_64 = ubuntu1904x86_64 super; };
    rpmDistros = super.vmTools.rpmDistros // { fedora30x86_64 = fedora30x86_64 super; };
    diskImages =
      (super.lib.mapAttrs (name: as: super.vmTools.makeImageFromRPMDist as) rpmDistros) //
      (super.lib.mapAttrs (name: as: super.vmTools.makeImageFromDebDist as) debDistros);
  };
}
