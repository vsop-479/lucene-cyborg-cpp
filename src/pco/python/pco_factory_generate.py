import os

from gen_utils import *
from hash_table_size_utils import find_hash_table_size
from pco_model import *


class _FactoryMeta:
    def __init__(self, target: PreCompiledTargetConfiguration):
        self.target: PreCompiledTargetConfiguration = target
        self.dest_dir = None
        self.class_name = None
        self.method_names = []
        self.parameter_type_infos = []
        self.custom_type_infos = []

    def get_return_type_str(self, require_full_signature: bool = True):
        if require_full_signature:
            return f'lucene::cyborg::util::ManagedPtr<{self.target.target_interface.get_signature()}>'
        else:
            return f'ManagedPtr<{self.target.target_interface.type_name}>'

    def get_factory_parameter_strs(self, ctor_idx: int):
        parameter_strs = []
        for t in self.parameter_type_infos[ctor_idx] + self.custom_type_infos[ctor_idx]:
            if t.is_object:
                parameter_strs.append(f'ManagedPtr<{t.type_name}>')

        return parameter_strs + ['ManagedPtr<LinearAllocator>']

    def get_factory_parameter_value_str_with_casting(self,
                                                     table: PreCompiledUnitRoutingTable,
                                                     row: list[PreCompiledUnitRoutingCell]):
        param_strs = []
        idx = 0

        for cell in row:
            # enum + native values are already included in template, we don't need to bother to pass them.
            type_info = cell.get_type_info()
            if type_info.is_object:
                param_strs.append(f'(({type_info.type_name}*) param_{idx})')
            idx += 1

        for _ in table.constructor.custom_parameters:
            param_strs.append(f'param_{idx}')
            idx += 1

        return param_strs

    def get_factory_parameter_value_strs(self, table: PreCompiledUnitRoutingTable):
        idx = 0
        param_strs = []
        for param in table.constructor.parameters:
            # enum + native values are already included in template, we don't need to bother to pass them.
            if param.type_info.is_object:
                param_strs.append(f'param_{idx}')
            idx += 1
        for _ in table.constructor.custom_parameters:
            param_strs.append(f'param_{idx}')
            idx += 1
        return param_strs + ['allocator']

    def get_parameter_strs(self, ctor_index: int, require_full_signature: bool = True):
        parameter_strs = []
        for t in self.parameter_type_infos[ctor_index] + self.custom_type_infos[ctor_index]:
            if t.is_object:
                if require_full_signature:
                    # used in hpp
                    parameter_strs.append(f'lucene::cyborg::util::ManagedPtr<{t.get_signature()}>')
                else:
                    # used in cpp
                    parameter_strs.append(f'ManagedPtr<{t.type_name}>')
            elif t.is_enum:
                if require_full_signature:
                    # used in hpp
                    parameter_strs.append(t.get_signature())
                else:
                    # used in cpp
                    parameter_strs.append(t.type_name)
            else:
                parameter_strs.append(t.get_signature())

        if require_full_signature:
            # used in hpp
            parameter_strs.append('lucene::cyborg::util::ManagedPtr<lucene::cyborg::util::LinearAllocator>')
        else:
            # used in cpp
            parameter_strs.append('ManagedPtr<LinearAllocator>')

        return parameter_strs


class _TableElement:
    def __init__(self, row: list[PreCompiledUnitRoutingCell], factory_func_name: str):
        self.factory_func_name: str = factory_func_name
        self.row = row
        self.indices = [cell.value.type_index for cell in row]
        # sort_key is used for sort all rows in a table.
        self.sort_key = "|".join([str(i) for i in self.indices])
        self.hash_val = 0
        PRIME = 31
        m = 1
        for type_index in self.indices:
            self.hash_val += m * type_index
            m *= PRIME
        self.hash_val += m

        # will be finalized later
        self.next_index = None
        self.position_in_tale = None


def _gen_one_factory_method(f, meta: _FactoryMeta, ctor_idx: int, table: PreCompiledUnitRoutingTable):
    # 0. Starting comment
    w(f, '//')
    w(f, f'// Constructor-{ctor_idx}, Suffix=[{table.constructor.method_name_suffix}]')
    w(f, f'// Target type: [{table.target.target_type_info.get_signature()}]')
    w(f, f'// Target interface type: [{table.target.target_interface.get_signature()}]')
    w(f, '//')
    newline(f)

    # 1. Hash table
    num_elements = len(table.rows)
    num_parameters = len(table.constructor.parameters)
    hash_table_size = find_hash_table_size(num_elements)
    hash_table_size_var_name = f'CTOR_{ctor_idx}_HASH_TABLE_SIZE'
    w(f, f'constexpr int {hash_table_size_var_name} = {hash_table_size};')

    # 1.1. Define Ctor{idx}TableKey = std::tuple<int, int, ...>
    key_alias = f'{table.target.target_type_info.type_name}Ctor{ctor_idx}TableKey'
    w(f, f'using {key_alias} = PcoTableKey<{num_parameters}>;')

    # 1.2. Define factory function type
    factory_function_alias = f'{table.target.target_type_info.type_name}Ctor{ctor_idx}FactoryFunction'
    factory_parameters_strs = meta.get_factory_parameter_strs(ctor_idx)
    w(f,
      f'using {factory_function_alias} = '
      f'{meta.get_return_type_str(False)} (*)({", ".join(factory_parameters_strs)});')

    # 1.3. Define factory function: a row maps to a factory function
    newline(f)
    all_params = [f'{param} param_{idx}' for idx, param in enumerate(factory_parameters_strs)]
    table_elements = []
    for func_idx, row in enumerate(table.rows):
        factory_func_name = f'{table.target.target_type_info.type_name}_ctor_{ctor_idx}_factory_func_{func_idx}'

        table_element = _TableElement(row, factory_func_name)
        table_elements.append(table_element)

        w(f, f'{meta.get_return_type_str(False)} {factory_func_name}(')
        w(f, ", ".join(all_params), 2)
        # last param is always allocator
        w(f, ') {')
        w(f, f'auto allocator = param_{len(all_params) - 1};', 2)
        newline(f)

        w(f, f'return allocator->allocate_object<{table.compiled_type_infos[func_idx].type_name}>('
             f'{", ".join(meta.get_factory_parameter_value_str_with_casting(table, row))});', 2)

        w(f, '}')
        newline(f)

    # 1.3.1. define 'delta' in hash function
    table_elements.sort(key=lambda e: e.sort_key)
    delta_in_hash_function = hash_table_size - (table_elements[0].hash_val % hash_table_size)
    for table_element in table_elements:
        table_element.hash_val += delta_in_hash_function

    # 1.4. Define table cell : Ctor{idx}TableCell
    # template<int N, typename FactoryFunc, typename Key>
    # struct PcoTableCell;
    table_cell_name = f'{table.target.target_type_info.type_name}Ctor{ctor_idx}TableCell'
    w(f, f'using {table_cell_name}= PcoTableCell<{factory_function_alias}, {key_alias}>;')

    # 1.5. Define position
    hash_table = [None] * hash_table_size
    for idx, table_elem in enumerate(table_elements):
        pos = table_elem.hash_val % hash_table_size
        while True:
            if hash_table[pos] is None:
                table_elem.position_in_table = pos
                hash_table[pos] = table_elem
                break
            pos = (pos + 1) % hash_table_size
    valid_table_elems_sorted = sorted([e for e in hash_table if e is not None], key=lambda e: e.position_in_table)
    for idx, table_elem in enumerate(valid_table_elems_sorted):
        valid_table_elems_sorted[idx - 1].next_index = table_elem.position_in_table

    # 1.6. define static has_table
    hash_table_name = f'{table.target.target_type_info.type_name}_CTOR_{ctor_idx}_TABLE'
    w(f, f'{table_cell_name} {hash_table_name}[{hash_table_size_var_name}] = {{')
    table_init_params = []
    for table_elem in hash_table:
        if table_elem is None:
            table_init_params.append(f'{table_cell_name}()')
        else:
            # PcoTableCell(int _next_index, FactoryFunc _factory_func, Key &&_key)
            table_init_params.append(
                f'{table_cell_name}('
                f'{table_elem.next_index}, '
                f'{table_elem.factory_func_name}, '
                f'{key_alias}({{' + f'{", ".join([str(idx) for idx in table_elem.indices])}' + f'}}, {str(delta_in_hash_function)}))')
    w(f, ", ".join(table_init_params), 2)
    w(f, '};')

    newline(f)

    # 2. Body
    w(f, f'{meta.get_return_type_str(False)} {meta.class_name}::{meta.method_names[ctor_idx]}(')
    param_strs = [f'{param_str} param_{idx}' for idx, param_str in enumerate(meta.get_parameter_strs(ctor_idx, False))]
    w(f, f'{", ".join(param_strs)}) {{', 2)

    w(f, f'auto allocator = param_{len(param_strs) - 1};', 2)
    newline(f)

    w(f, f'const auto key = {key_alias} ({{', 2)
    key_init_params = []
    for idx, param_type_info in enumerate(meta.parameter_type_infos[ctor_idx]):
        param_idx = f'param_{idx}'
        if param_type_info.is_object:
            key_init_params.append(
                f'{param_idx} != nullptr ? '
                f'(((PcoTypeIndexSupport*) {param_idx})->get_pco_type_index()) '
                f': ({PCO_NULL_TYPE.type_index})')
        else:
            key_init_params.append(f'PcoNativeTypeIndex<{param_type_info.get_signature()}>::get_index({param_idx})')
    w(f, ", ".join(key_init_params), 4)
    w(f, '},', 2)
    w(f, f"{str(delta_in_hash_function)});", 2)
    newline(f)

    w(f,
      f'auto factory = PcoTableLookUp::look_up<{key_alias}, {table_cell_name}, {hash_table_size_var_name}>(key, &{hash_table_name}[0]);',
      2)
    newline(f)

    # last param is always allocator
    w(f, f'return factory({", ".join(meta.get_factory_parameter_value_strs(table))});', 2)
    w(f, '}')
    newline(f)


def _gen_factory_cpp(build_config: PreCompiledTargetBuildConfiguration,
                     target: PreCompiledTargetConfiguration,
                     factory_meta: _FactoryMeta):
    cpp_file_name = factory_meta.class_name + ".cpp"
    cpp_file = os.path.join(factory_meta.dest_dir, cpp_file_name)

    with open(cpp_file, 'wt', encoding='utf-8') as f:
        w(f, '#include <utility>')
        w(f, f'#include "{factory_meta.class_name}.hpp"')
        w(f, '#include "PcoTableModel.hpp"')
        w(f, '#include "../typeindex/PcoTypeIndexSupport.hpp"')
        w(f, '#include "../typeindex/PcoNativeTypeIndex.hpp"')

        include_types = set()
        include_types.add(target.target_type_info)

        for table in target.tables:
            for row in table.rows:
                for cell in row:
                    include_types.add(cell.get_type_info())
                    if isinstance(cell.get_type_info(), ObjectTypeInfo):
                        for tmpl_param_type_info in cell.get_type_info().template_parameter_type_infos:
                            include_types.add(tmpl_param_type_info)

            for custom_param in table.constructor.custom_parameters:
                include_types.add(custom_param.type_info)

        # For template types, they can have same header.
        # Ex: Foo<A, B> -> Foo.hpp, Foo<C, D> -> Foo.hpp
        # therefore, we need explicitly filter unique headers to include.
        for header in {t.header for t in include_types}:
            w(f, f'#include "../../{header}"')
        newline(f)

        include_types.update([target.target_interface])
        using_declarations = set()
        for nm_and_type in sorted([t.get_signature() for t in include_types]):
            # Foo<A, B> -> Foo
            bracket_idx = nm_and_type.find('<')
            if bracket_idx != -1:
                nm_and_type = nm_and_type[0:bracket_idx]
            using_declarations.add(f'using {nm_and_type};')
        using_declarations.update(['using lucene::cyborg::util::ManagedPtr;',
                                   'using lucene::cyborg::util::LinearAllocator;'])

        for using in sorted(using_declarations):
            w(f, using)
        newline(f)

        w(f, 'namespace lucene::cyborg::pco {')
        newline(f)

        for idx, table in enumerate(target.tables):
            _gen_one_factory_method(f, factory_meta, idx, table)

        w(f, '}  // namespace lucene::cyborg::pco')


def _gen_factory_header(build_config: PreCompiledTargetBuildConfiguration,
                        target: PreCompiledTargetConfiguration):
    meta = _FactoryMeta(target)
    dest_dir = os.path.join(build_config.base_dir_path, "src", "pco", "factory")
    meta.dest_dir = dest_dir
    # Ex: aaa::bbb::ccc::FooBar -> PcoTargetClassFactoryOfAaaBbbCccFooBar::create_for_XXX
    # header guard ex:
    #   #ifndef LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGCODECSV90LUCENE90IMPACTSENUM_HPP_
    class_name = "PcoTargetClassFactoryOf"
    class_name += "".join([nm[0].upper() + nm[1:] for nm in target.target_type_info.namespace.split("::")])
    class_name += target.target_type_info.type_name
    meta.class_name = class_name
    header_name = class_name + ".hpp"
    header = os.path.join(dest_dir, header_name)

    with open(header, 'wt', encoding='utf-8') as f:
        header_guard = 'LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_' \
                       + header_name.replace(".", "_").upper() + "_"
        w(f, f'#ifndef {header_guard}')
        w(f, f'#define {header_guard}')
        newline(f)

        include_headers = set()
        include_headers.add("util/PointerTypes.hpp")
        include_headers.add("util/LinearAllocator.hpp")

        include_headers.add(target.target_interface.header)
        for table in target.tables:
            ctor = table.constructor
            for param_and_candidates in ctor.parameter_and_candidates:
                include_headers.add(param_and_candidates[0].type_info.header)
            for custom_param in ctor.custom_parameters:
                include_headers.add(custom_param.type_info.header)
        for include_header in include_headers:
            w(f, f'#include "../../{include_header}"')
        newline(f)

        w(f, 'namespace lucene::cyborg::pco {')
        newline(f)

        w(f, f'struct {class_name} {{')

        for ctor_idx, table in enumerate(target.tables):
            ctor = table.constructor
            meta.parameter_type_infos.append([param.type_info for param in ctor.parameters])
            meta.custom_type_infos.append([param.type_info for param in ctor.custom_parameters])

            method_name = 'create'
            if len(ctor.method_name_suffix) > 0:
                method_name = f'create_for_{ctor.method_name_suffix}'

            w(f, f'static {meta.get_return_type_str()} {method_name}(', 2)
            meta.method_names.append(method_name)

            parameter_strs = meta.get_parameter_strs(ctor_idx)
            for idx, param_str in enumerate(parameter_strs):
                if idx != (len(parameter_strs) - 1):
                    param_str += ","
                else:
                    param_str += ");"
                w(f, param_str, 4)

        w(f, f'}};  // {class_name}')

        newline(f)
        w(f, '}  // namespace lucene::cyborg::pco')
        newline(f)
        w(f, f'#endif // {header_guard}')

    return meta


def gen_factory_for_all_targets(build_config: PreCompiledTargetBuildConfiguration,
                                targets: list[PreCompiledTargetConfiguration]):
    for target in targets:
        if target.gen_source:
            factory_meta = _gen_factory_header(build_config, target)
            _gen_factory_cpp(build_config, target, factory_meta)
