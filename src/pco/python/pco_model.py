class TypeInfo:
    def __init__(self, type_name: str):
        self.type_name = type_name
        self.is_native = False
        self.is_object = False
        self.is_enum = False
        self.header = None
        self.namespace = None

    def __eq__(self, other):
        if isinstance(other, TypeInfo):
            return (self.type_name == other.type_name
                    and self.is_native == other.is_native
                    and self.header == other.header
                    and self.namespace == other.namespace)

    def __hash__(self):
        return hash((self.type_name, self.is_native, self.header, self.namespace))

    def get_signature(self) -> str:
        if self.namespace is None:
            return self.type_name
        return f"{self.namespace}::{self.type_name}"


class NativeTypeInfo(TypeInfo):
    def __init__(self, type_name: str):
        TypeInfo.__init__(self, type_name)
        self.is_native = True

    def __eq__(self, other):
        if isinstance(other, NativeTypeInfo):
            return TypeInfo.__eq__(self, other)
        return False


BOOL_NATIVE_TYPE = NativeTypeInfo('bool')
INT16_NATIVE_TYPE = NativeTypeInfo('int16_t')
INT32_NATIVE_TYPE = NativeTypeInfo('int32_t')
INT64_NATIVE_TYPE = NativeTypeInfo('int64_t')
INT8_NATIVE_TYPE = NativeTypeInfo('int8_t')
UINT16_NATIVE_TYPE = NativeTypeInfo('uint16_t')
UINT32_NATIVE_TYPE = NativeTypeInfo('uint32_t')
UINT64_NATIVE_TYPE = NativeTypeInfo('uint64_t')
UINT8_NATIVE_TYPE = NativeTypeInfo('uint8_t')


class EnumTypeInfo(TypeInfo):
    def __init__(self, type_name: str, header: str, namespace: str):
        TypeInfo.__init__(self, type_name)
        self.header = header
        self.namespace = namespace
        self.is_enum = True


class ObjectTypeInfo(TypeInfo):
    def __init__(self, type_name: str, header: str, namespace: str,
                 template_parameter_type_infos: list[TypeInfo] = []):
        TypeInfo.__init__(self, type_name)
        self.header = header
        self.namespace = namespace
        self.is_object = True
        self.template_parameter_type_infos = template_parameter_type_infos
        # Will be set during indices assignment.
        self.type_index = -1

    def __eq__(self, other):
        if isinstance(other, ObjectTypeInfo):
            return (TypeInfo.__eq__(self, other)
                    and self.header == other.header
                    and self.namespace == other.namespace)
        return False

    def __hash__(self):
        return TypeInfo.__hash__(self) * 31 + hash((self.header, self.namespace))


PCO_NULL_TYPE = ObjectTypeInfo(
    "PcoNullType",
    r"pco/util/PcoNullType.hpp",
    "lucene::cyborg::pco"
)


class PointerWrapper:
    MANAGED_PTR = "ManagedPtr"


class Parameter:
    def __init__(self,
                 type_info: TypeInfo,
                 is_const: bool = False,
                 reference: str = "",
                 default_value: str = "",
                 ptr_wrapper: str = ""):
        self.type_info = type_info
        self.is_const = is_const
        self.reference = reference
        self.default_value = default_value
        self.ptr_wrapper = ptr_wrapper

    def __eq__(self, other):
        if isinstance(other, Parameter):
            return self.type_info == other.type_info and self.is_const == other.is_const
        return False

    def __hash__(self):
        return self.type_info.__hash__() * 31 + hash(self.is_const)


class NativeOrEnumValue:
    def __init__(self, type_info: TypeInfo, value):
        self.type_info = type_info
        self.value = value
        # Will be set during indices assignment.
        self.type_index = -1

    def __eq__(self, other):
        if isinstance(other, NativeOrEnumValue):
            return self.type_info == other.type_info and self.value == other.value
        return False

    def __hash__(self):
        return self.type_info.__hash__() * 31 + hash(self.value)


class PreCompiledTargetBuildConfiguration:
    def __init__(self, base_dir_path: str):
        self.base_dir_path = base_dir_path
        self.type_index_start = 0


class PreCompiledTargetConstructor:
    def __init__(self,
                 method_name_suffix: str,
                 parameter_and_candidates: list[tuple[Parameter, list]],
                 custom_parameters: list[Parameter] = []):
        self.method_name_suffix: str = method_name_suffix
        self.parameter_and_candidates: list[tuple[Parameter, list]] = parameter_and_candidates
        self.parameters = []
        for param, _ in parameter_and_candidates:
            self.parameters.append(param)
        self.custom_parameters: list[Parameter] = custom_parameters


class PcoGeneratedSource:
    def __init__(self, factory_header: str, factory_impl_cpp: str):
        self.factory_header = factory_header
        self.factory_impl_cpp = factory_impl_cpp


class PreCompiledTargetConfiguration:
    def __init__(self,
                 target_interface: TypeInfo,
                 target_type_info: TypeInfo,
                 parameters: list[Parameter],
                 headers_to_include: list[str],
                 constructors: list[PreCompiledTargetConstructor],
                 gen_source: bool = True):
        self.target_interface = target_interface
        self.target_type_info = target_type_info
        self.parameters = parameters
        self.headers_to_include = headers_to_include
        self.constructors = constructors
        self.factory_header = None
        self.factory_impl_cpp = None
        self.pco_generated_source = None
        self.gen_source = gen_source
        #
        # Below variables will be set during the table build
        #
        self.index: int = -1
        self.tables: list[PreCompiledUnitRoutingTable] = None


class PreCompiledUnitRoutingCell:
    def __init__(self, parameter: Parameter, value):
        self.parameter: Parameter = parameter
        self.value = value

    def get_type_info(self):
        if self.value is None:
            return None
        elif isinstance(self.value, NativeOrEnumValue):
            return self.value.type_info
        else:
            return self.value


class PreCompiledUnitRoutingTable:
    def __init__(self,
                 target: PreCompiledTargetConfiguration,
                 constructor: PreCompiledTargetConstructor,
                 rows: list[list[PreCompiledUnitRoutingCell]],
                 signatures: list[TypeInfo]):
        self.target: PreCompiledTargetConfiguration = target
        self.constructor: PreCompiledTargetConstructor = constructor
        self.rows: list[list[PreCompiledUnitRoutingCell]] = rows
        self.compiled_type_infos: list[TypeInfo] = signatures
