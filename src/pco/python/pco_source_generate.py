from pco_factory_generate import *
from type_index_source_generate import *


def _validate(build_config: PreCompiledTargetBuildConfiguration,
              targets: list[PreCompiledTargetConfiguration]):
    # TODO
    pass


def _do_topological_sort_targets(target: PreCompiledTargetConfiguration, visited, sorted_list):
    visited.add(target.index)
    for ctor in target.constructors:
        for param_and_candidates in ctor.parameter_and_candidates:
            candidates = param_and_candidates[1]
            for candidate in candidates:
                if isinstance(candidate, PreCompiledTargetConfiguration):
                    if candidate.index not in visited:
                        _do_topological_sort_targets(candidate, visited, sorted_list)
    sorted_list.append(target)


def _assign_index_to_target(target: PreCompiledTargetConfiguration,
                            index: int,
                            all_targets: list[PreCompiledTargetConfiguration]):
    if target.index != -1:
        return index

    for ctor in target.constructors:
        for param_and_candidates in ctor.parameter_and_candidates:
            candidates = param_and_candidates[1]
            for candidate in candidates:
                if isinstance(candidate, PreCompiledTargetConfiguration):
                    index = _assign_index_to_target(candidate, index, all_targets)

    target.index = index
    all_targets.append(target)
    return index + 1


def _topological_sort_targets(targets: list[PreCompiledTargetConfiguration]):
    all_targets: list[PreCompiledTargetConfiguration] = []
    for target in targets:
        _assign_index_to_target(target, 0, all_targets)
    return all_targets


def _build_table_obj(target: PreCompiledTargetConfiguration,
                     parameters: list[Parameter],
                     candidates,
                     idx: int,
                     one_row: list[PreCompiledUnitRoutingCell],
                     rows: list[list[PreCompiledUnitRoutingCell]],
                     compiled_type_infos: list[TypeInfo]):
    if idx >= len(candidates):
        rows.append(one_row.copy())
        compiled_type_info = []
        for param, cell in zip(parameters, one_row):
            if param.type_info.is_native:
                # Ex: 4
                compiled_type_info.append(cell.value.value)
            elif param.type_info.is_enum:
                # Ex: IndexOptions::XXX
                compiled_type_info.append(cell.value.type_info.get_signature() + f"::{cell.value.value}")
            elif param.type_info.is_object:
                # Ex: aaa::bbb::ccc::FooBar
                compiled_type_info.append(cell.value.get_signature())

        compiled_type_infos.append(
            ObjectTypeInfo(f"{target.target_type_info.type_name}<" + ", ".join(compiled_type_info) + ">",
                           target.target_type_info.header,
                           target.target_type_info.namespace,
                           [cell.get_type_info() for cell in one_row]))
        return

    parameter = parameters[idx]

    # `value` is one of [TypeInfo, NativeOrEnumValue, PreCompiledTargetConfiguration]
    for value in candidates[idx]:
        if isinstance(value, PreCompiledTargetConfiguration):
            for table in value.tables:
                for compiled_type_info in table.compiled_type_infos:
                    one_row.append(PreCompiledUnitRoutingCell(parameter, compiled_type_info))

                    _build_table_obj(target,
                                     parameters,
                                     candidates,
                                     idx + 1,
                                     one_row,
                                     rows,
                                     compiled_type_infos)
                    one_row.pop()
        else:
            one_row.append(PreCompiledUnitRoutingCell(
                parameter, value))
            _build_table_obj(target,
                             parameters,
                             candidates,
                             idx + 1,
                             one_row,
                             rows,
                             compiled_type_infos)
            one_row.pop()


def _build_one_table(target: PreCompiledTargetConfiguration, ctor: PreCompiledTargetConstructor):
    parameters = target.parameters
    candidate_types = []
    for param in parameters:
        candidates = []
        candidate_types.append(candidates)

        if param.type_info.is_object:
            # Add parameter interface type (e.g. abstract type)
            candidates.append(param.type_info)

        for apply_param in ctor.parameter_and_candidates:
            if param == apply_param[0]:
                candidates += apply_param[1]

        if (param.type_info.is_enum or param.type_info.is_native) and len(candidates) <= 0:
            raise RuntimeError("Empty candidates for parameter=" + param.type_info.type_name)

    table = []
    compiled_type_infos = []
    _build_table_obj(target,
                     parameters,
                     candidate_types,
                     0,
                     [],
                     table,
                     compiled_type_infos)

    return PreCompiledUnitRoutingTable(target, ctor, table, compiled_type_infos)


def _build_tables(targets: list[PreCompiledTargetConfiguration]):
    targets = _topological_sort_targets(targets)
    for target in targets:
        target.tables = []
        for ctor in target.constructors:
            table = _build_one_table(target, ctor)
            target.tables.append(table)
    return targets


def gen_pco_sources(build_config: PreCompiledTargetBuildConfiguration,
                    targets: list[PreCompiledTargetConfiguration]):
    _validate(build_config, targets)
    all_targets = _build_tables(targets)
    gen_global_native_type_index(build_config, all_targets)
    gen_global_object_type_index(build_config, all_targets)

    gen_factory_for_all_targets(build_config, all_targets)
