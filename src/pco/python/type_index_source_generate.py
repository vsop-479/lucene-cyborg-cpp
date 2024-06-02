import os
from typing import Dict

from gen_utils import *
from pco_model import *

global_index = 0


def _do_collect_type_and_values(target: PreCompiledTargetBuildConfiguration,
                                type_info_predicate,
                                type_map):
    parameters = target.parameters
    for table in target.tables:
        for row in table.rows:
            cell_idx = 0
            for cell in row:
                if not type_info_predicate(cell.get_type_info()):
                    continue

                if parameters[cell_idx].type_info not in type_map:
                    type_map[parameters[cell_idx].type_info] = set()
                type_map[parameters[cell_idx].type_info].add(cell.value)
                cell_idx += 1


def _collect_type_and_values(targets: list[PreCompiledTargetConfiguration],
                             type_info_predicate):
    type_to_values_map = dict()

    for target in targets:
        _do_collect_type_and_values(target, type_info_predicate, type_to_values_map)

    return type_to_values_map


def gen_global_native_type_index(build_config: PreCompiledTargetBuildConfiguration,
                                 targets: list[PreCompiledTargetConfiguration]):
    global global_index

    type_to_values = _collect_type_and_values(targets, lambda type_info: type_info.is_enum or type_info.is_native)
    include_headers = set()
    for values in type_to_values.values():
        for value in values:
            include_headers.add(value.type_info.header)

    dest_dir = os.path.join(build_config.base_dir_path, "src", "pco", "typeindex")
    header = os.path.join(dest_dir, "PcoNativeTypeIndex.hpp")
    with open(header, 'wt', encoding='utf-8') as f:
        w(f, '#ifndef LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCONATIVETYPEINDEX_HPP_')
        w(f, '#define LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCONATIVETYPEINDEX_HPP_')
        newline(f)

        # include headers
        w(f, '#include <cstdint>')
        for include_header in include_headers:
            w(f, f'#include "../../{include_header}"')
        newline(f)

        w(f, 'namespace lucene::cyborg::pco {')
        newline(f)
        w(f, 'template <typename>')
        w(f, 'struct PcoNativeTypeIndex;')
        newline(f, 3)

        # make template specialization
        for native_type in type_to_values:
            w(f, 'template <>')
            w(f, f'struct PcoNativeTypeIndex<{native_type.get_signature()}> {{')
            w(f, f'static int32_t get_index(const {native_type.get_signature()} value) noexcept {{', 2)
            comparisons = []
            for native_value in type_to_values[native_type]:
                comparisons.append(('value',
                                    str(native_value.value) if native_type.is_native
                                    else f'{native_type.get_signature()}::{native_value.value}',
                                    f'return {global_index};'))
                native_value.type_index = global_index
                global_index += 1
            write_if_else(f, comparisons, 4, 'return -1;')
            w(f, '}', 2)
            w(f, f'}};  // {native_type.get_signature()}')
            newline(f, 3)

        w(f, '}  // namespace lucene::cyborg::pco')
        newline(f)
        w(f, '#endif //LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCONATIVETYPEINDEX_HPP_')


def _gen_global_templated_type_index_header(build_config: PreCompiledTargetBuildConfiguration,
                                            targets: list[PreCompiledTargetConfiguration]):
    global global_index

    type_to_values_map = _collect_type_and_values(
        targets,
        lambda type_info: type_info.is_object and '<' in type_info.type_name)

    dest_dir = os.path.join(build_config.base_dir_path, "src", "pco", "typeindex")
    header = os.path.join(dest_dir, "PcoTemplatedObjectTypeIndex.hpp")
    with open(header, 'wt', encoding='utf-8') as f:
        w(f, '#ifndef LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCOTEMPLATEDOBJECTTYPEINDEX_HPP_')
        w(f, '#define LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCOTEMPLATEDOBJECTTYPEINDEX_HPP_')
        newline(f)
        w(f, 'namespace lucene::cyborg::pco {')
        newline(f)
        w(f, 'struct PcoTemplatedObjectTypeIndex {')
        w(f, 'static bool assign_type_index();', 2)
        w(f, '};  // PcoTemplatedObjectTypeIndex')
        newline(f, 3)
        w(f, '}  // namespace lucene::cyborg::pco')
        newline(f)
        w(f, '#endif //LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCOTEMPLATEDOBJECTTYPEINDEX_HPP_')

    return type_to_values_map


def _gen_global_templated_type_index_cpp(build_config: PreCompiledTargetBuildConfiguration,
                                         targets: Dict[TypeInfo, list[TypeInfo]]):
    global global_index

    # targets: map[TypeInfo, list[TypeInfo]]
    dest_dir = os.path.join(build_config.base_dir_path, "src", "pco", "typeindex")
    header = os.path.join(dest_dir, "PcoTemplatedObjectTypeIndex.cpp")
    with open(header, 'wt', encoding='utf-8') as f:
        w(f, '#include "PcoTemplatedObjectTypeIndex.hpp"')

        include_headers = set()
        for type_info in targets:
            include_headers.add(type_info.header)
            include_headers.update([candidate.header for candidate in targets[type_info]])

        for include_header in include_headers:
            w(f, f'#include "../../{include_header}"')
        newline(f)

        w(f, '// PCO_INDEX definitions')
        for type_info in targets:
            w(f, f'// Parameter interface: [{type_info.get_signature()}]')
            for candidate in targets[type_info]:
                w(f, f'template<> uint64_t {candidate.get_signature()}::PCO_INDEX = {global_index};')
                candidate.type_index = global_index
                global_index += 1
            newline(f)

        newline(f)
        w(f, 'bool lucene::cyborg::pco::PcoTemplatedObjectTypeIndex::assign_type_index() {')
        w(f, 'return true;', 2)
        w(f, '}')


def _gen_global_templated_type_index(build_config: PreCompiledTargetBuildConfiguration,
                                     targets: list[PreCompiledTargetConfiguration]):
    type_to_values_map = _gen_global_templated_type_index_header(build_config, targets)
    _gen_global_templated_type_index_cpp(build_config, type_to_values_map)


def _gen_global_non_templated_type_index(build_config: PreCompiledTargetBuildConfiguration,
                                         targets: list[PreCompiledTargetConfiguration]):
    global global_index

    type_to_values_map = _collect_type_and_values(targets, lambda type_info: type_info.is_object
                                                                             and '<' not in type_info.type_name)
    dest_dir = os.path.join(build_config.base_dir_path, "src", "pco", "typeindex")
    header = os.path.join(dest_dir, "PcoObjectTypeIndex.hpp")
    with open(header, 'wt', encoding='utf-8') as f:
        w(f, '#ifndef LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCOOBJECTTYPEINDEX_HPP_')
        w(f, '#define LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCOOBJECTTYPEINDEX_HPP_')
        newline(f)
        w(f, '#include <cstdint>')
        newline(f)
        w(f, 'namespace lucene::cyborg::pco {')
        newline(f)
        w(f, 'struct PcoObjectTypeIndex {')

        unique_type_infos = set()
        for type_infos in type_to_values_map.values():
            for type_info in type_infos:
                unique_type_infos.add(type_info)

        for type_info in unique_type_infos:
            namespace_part = "_".join([part.upper() for part in type_info.namespace.split("::")])
            type_name = []
            start = 0
            for i in range(len(type_info.type_name)):
                if i > 0 and type_info.type_name[i].isupper():
                    type_name.append(type_info.type_name[start:i].upper())
                    start = i
            type_name.append(type_info.type_name[start:].upper())
            type_name = "_".join(type_name)

            # Ex: aaa::bbb::ccc:FooBar -> AAA_BBB_CCC_FOO_BAR
            var_name = f"{namespace_part}_{type_name}"
            w(f, f'static constexpr int32_t {var_name} = {global_index};', 2)
            type_info.type_index = global_index
            global_index += 1

        w(f, '};  // PcoObjectTypeIndex')
        newline(f, 3)
        w(f, '}  // namespace lucene::cyborg::pco')
        newline(f)
        w(f, '#endif //LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCOOBJECTTYPEINDEX_HPP_')


def gen_global_object_type_index(build_config: PreCompiledTargetBuildConfiguration,
                                 targets: list[PreCompiledTargetConfiguration]):
    global global_index

    _gen_global_non_templated_type_index(build_config, targets)
    _gen_global_templated_type_index(build_config, targets)
