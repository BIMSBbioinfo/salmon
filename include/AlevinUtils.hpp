#ifndef __ALEVIN_UTILS_HPP__
#define __ALEVIN_UTILS_HPP__

#include <algorithm>
#include <atomic>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <exception>
#include <functional>
#include <iterator>
#include <map>
#include <mutex>
#include <queue>
#include <random>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <string>

#include "spdlog/spdlog.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/variant.hpp>
#include <boost/any.hpp>

#include "cereal/archives/json.hpp"

#include "AlevinOpts.hpp"
#include "SingleCellProtocols.hpp"
#include "BarcodeGroup.hpp"

#include "SalmonConfig.hpp"
#include "SalmonUtils.hpp"

namespace alevin{
  namespace utils{

    namespace apt = alevin::protocols;

    constexpr uint32_t uint32_max = std::numeric_limits<uint32_t>::max();

    void getIndelNeighbors(
                           const std::string& barcodeSeq,
                           std::unordered_set<uint32_t>& neighbors);

    void findNeighbors(size_t length,
                       const std::string& barcode,
                       std::unordered_set<std::string>& neighbors);

    template <typename ProtocolT>
    bool sequenceCheck(std::string barcode,
                       AlevinOpts<ProtocolT>& aopt,
                       std::mutex& iomutex,
                       Sequence seq = Sequence::BARCODE);

    template <typename ProtocolT>
    bool processAlevinOpts(AlevinOpts<ProtocolT>& aopt,
                           boost::program_options::variables_map& vm);

    template <typename ProtocolT>
    bool extractUMI(std::string& read,
                    ProtocolT& pt,
                    std::string& umi);

    template <typename ProtocolT>
    bool extractBarcode(std::string& read,
                        ProtocolT& pt,
                        std::string& bc);

    template <typename OrderedOptionsT>
    bool writeCmdInfo(boost::filesystem::path cmdInfoPath,
                      OrderedOptionsT& orderedOptions) {
      namespace bfs = boost::filesystem;
      std::ofstream os(cmdInfoPath.string());
      cereal::JSONOutputArchive oa(os);
      oa(cereal::make_nvp("Salmon_version:", std::string(salmon::version)));
      for (auto& opt : orderedOptions.options) {
        if (opt.value.size() == 1) {
          oa(cereal::make_nvp(opt.string_key, opt.value.front()));
        } else {
          oa(cereal::make_nvp(opt.string_key, opt.value));
        }
      }
      return true;
    }

  }
}
#endif // __ALEVIN_UTILS_HPP__
