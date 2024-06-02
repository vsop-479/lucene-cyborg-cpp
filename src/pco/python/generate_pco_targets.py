from pco_source_generate import *

#####################
# Type definitions ##
#####################

bulk_scorer = ObjectTypeInfo(
    "BulkScorer",
    r"search/BulkScorer.hpp",
    "lucene::cyborg::search")

doc_id_set_iterator = ObjectTypeInfo(
    "DocIdSetIterator",
    r"search/DocIdSetIterator.hpp",
    "lucene::cyborg::search")

two_phase_iterator = ObjectTypeInfo(
    "TwoPhaseIterator",
    r"search/TwoPhaseIterator.hpp",
    "lucene::cyborg::search"
)

bits = ObjectTypeInfo(
    "Bits",
    r"util/Bits.hpp",
    "lucene::cyborg::util"
)

leaf_collector = ObjectTypeInfo(
    "LeafCollector",
    r"search/LeafCollector.hpp",
    "lucene::cyborg::search"
)

scorer = ObjectTypeInfo(
    "Scorer",
    r"search/Scorer.hpp",
    "lucene::cyborg::search"
)

# DefaultBulkScorer for search
default_bulk_scorer = ObjectTypeInfo(
    "DefaultBulkScorerPco",
    r"search/DefaultBulkScorerPco.hpp",
    "lucene::cyborg::search")

impacts_enum = ObjectTypeInfo(
    "ImpactsEnum",
    r"index/ImpactsEnum.hpp",
    "lucene::cyborg::index"
)

postings_enum = ObjectTypeInfo(
    "PostingsEnum",
    r"index/PostingsEnum.hpp",
    "lucene::cyborg::index"
)

lucene90_impacts_enum = ObjectTypeInfo(
    "Lucene90ImpactsEnum",
    r"codecs/lucene90/Lucene90ImpactsEnum.hpp",
    "lucene::cyborg::codecs::v90"
)

lucene90_postings_enum = ObjectTypeInfo(
    "Lucene90PostingsEnum",
    r"codecs/lucene90/Lucene90PostingsEnum.hpp",
    "lucene::cyborg::codecs::v90"
)

impacts_disi_base = ObjectTypeInfo(
    "ImpactsDISIPcoBase",
    r"search/ImpactsDISIPco.hpp",
    "lucene::cyborg::search"
)

impacts_disi = ObjectTypeInfo(
    "ImpactsDISIPco",
    r"search/ImpactsDISIPco.hpp",
    "lucene::cyborg::search"
)

max_score_cache = ObjectTypeInfo(
    "MaxScoreCache",
    r"search/MaxScoreCache.hpp",
    "lucene::cyborg::search"
)

index_options = EnumTypeInfo(
    "IndexOptions",
    r"index/IndexOptions.hpp",
    "lucene::cyborg::index"
)

block_max_conjunction_scorer = ObjectTypeInfo(
    "BlockMaxConjunctionScorer",
    r"search/query/BlockMaxConjunctionScorer.hpp",
    "lucene::cyborg::search"
)

block_max_conjunction_scorer_iterator = ObjectTypeInfo(
    "BlockMaxConjunctionScorerIteratorPco",
    r"search/query/BlockMaxConjunctionScorerIteratorPco.hpp",
    "lucene::cyborg::search"
)

conjunction_disi = ObjectTypeInfo(
    "ConjunctionDisiPco",
    r"search/query/iterator/ConjunctionDisiPco.hpp",
    "lucene::cyborg::search::query"
)

conjunction_disi_iterators = ObjectTypeInfo(
    "LVector<util::ManagedPtr<search::DocIdSetIterator>>",
    r"util/LinearAllocatorAdaptorForSTL.hpp",
    "lucene::cyborg::util"
)

boolean_scorer_or_collector_replayer_base = ObjectTypeInfo(
    "BooleanScorerOrCollectorReplayerBase",
    r"search/query/BooleanScorer.hpp",
    "lucene::cyborg::search::query"
)

boolean_scorer_or_collector_replayer = ObjectTypeInfo(
    "BooleanScorerOrCollectorReplayer",
    r"search/query/BooleanScorer.hpp",
    "lucene::cyborg::search::query"
)

doc_id_consumer = ObjectTypeInfo(
    "DocIdConsumer",
    r"search/DocIdConsumer.hpp",
    "lucene::cyborg::search"
)

match_count_leaf_collector = ObjectTypeInfo(
    "MatchCountLeafCollector",
    r"/search/MatchCountCollectorManager.hpp",
    "lucene::cyborg::search"
)

boolean_scorer_or_collector = ObjectTypeInfo(
    "BooleanScorerOrCollector",
    r"search/query/BooleanScorer.hpp",
    "lucene::cyborg::search::query"
)

default_count_doc_id_stream = ObjectTypeInfo(
    "DefaultCountDocIdStream",
    r"search/DocIdStream.hpp",
    "lucene::cyborg::search"
)

#######################
# Target definitions ##
#######################

targets = []

boolean_scorer_or_collector_replayer_target = PreCompiledTargetConfiguration(
    boolean_scorer_or_collector_replayer_base,
    boolean_scorer_or_collector_replayer,
    parameters=[Parameter(doc_id_consumer)],
    headers_to_include=[],
    constructors=[
        PreCompiledTargetConstructor(
            "",
            parameter_and_candidates=[
                (Parameter(doc_id_consumer), [
                    match_count_leaf_collector,
                    default_count_doc_id_stream
                ])
            ],
            custom_parameters=[
                Parameter(boolean_scorer_or_collector)
            ]
        )
    ]
)

targets.append(boolean_scorer_or_collector_replayer_target)

lucene90_impacts_enum_target = PreCompiledTargetConfiguration(
    impacts_enum,
    lucene90_impacts_enum,
    parameters=[Parameter(index_options)],
    headers_to_include=[],
    constructors=[
        PreCompiledTargetConstructor(
            "",
            parameter_and_candidates=[
                (Parameter(index_options), [
                    NativeOrEnumValue(index_options, "NONE"),
                    NativeOrEnumValue(index_options, "DOCS"),
                    NativeOrEnumValue(index_options, "DOCS_AND_FREQS"),
                    NativeOrEnumValue(index_options, "DOCS_AND_FREQS_AND_POSITIONS"),
                    NativeOrEnumValue(index_options, "DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS"),
                ])
            ]
        )
    ],
    gen_source=False
)

lucene90_postings_enum_target = PreCompiledTargetConfiguration(
    postings_enum,
    lucene90_postings_enum,
    parameters=[
        Parameter(index_options)
    ],
    headers_to_include=[],
    constructors=[
        PreCompiledTargetConstructor(
            "",
            parameter_and_candidates=[
                (Parameter(index_options), [
                    NativeOrEnumValue(index_options, "NONE"),
                    NativeOrEnumValue(index_options, "DOCS"),
                    NativeOrEnumValue(index_options, "DOCS_AND_FREQS"),
                    NativeOrEnumValue(index_options, "DOCS_AND_FREQS_AND_POSITIONS"),
                    NativeOrEnumValue(index_options, "DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS"),
                ])
            ]
        )
    ],
    gen_source=False
)

conjunction_disi_target = PreCompiledTargetConfiguration(
    doc_id_set_iterator,
    conjunction_disi,
    parameters=[
        Parameter(doc_id_set_iterator),
        Parameter(doc_id_set_iterator),
    ],
    headers_to_include=[],
    constructors=[
        PreCompiledTargetConstructor(
            "",
            parameter_and_candidates=[
                (
                    Parameter(doc_id_set_iterator),
                    [
                        lucene90_postings_enum_target
                    ]
                ),
                (
                    Parameter(doc_id_set_iterator),
                    [
                        lucene90_postings_enum_target
                    ]
                ),
            ],
            custom_parameters=[
                Parameter(conjunction_disi_iterators)
            ]
        )
    ]
)

targets.append(conjunction_disi_target)

impacts_disi_target = PreCompiledTargetConfiguration(
    impacts_disi_base,
    impacts_disi,
    parameters=[
        Parameter(doc_id_set_iterator)
    ],
    headers_to_include=[
    ],
    constructors=[
        PreCompiledTargetConstructor(
            "",
            [
                (
                    Parameter(doc_id_set_iterator),
                    [
                        lucene90_impacts_enum_target
                    ]
                )],
            [
                Parameter(max_score_cache)
            ]
        )
    ]
)

targets.append(impacts_disi_target)

block_max_conjunction_scorer_iterator_target = PreCompiledTargetConfiguration(
    doc_id_set_iterator,
    block_max_conjunction_scorer_iterator,
    parameters=[
        Parameter(doc_id_set_iterator),
    ],
    headers_to_include=[

    ],
    constructors=[
        PreCompiledTargetConstructor(
            "search",
            [
                (
                    Parameter(doc_id_set_iterator),
                    [
                        lucene90_impacts_enum_target,
                    ]
                )
            ], [
                Parameter(block_max_conjunction_scorer)
            ]
        ),
    ]
)

targets.append(block_max_conjunction_scorer_iterator_target)

default_bulk_scorer_target = PreCompiledTargetConfiguration(
    bulk_scorer,
    default_bulk_scorer,
    parameters=[
        Parameter(doc_id_set_iterator),
        Parameter(two_phase_iterator),
        Parameter(bits)],
    headers_to_include=[
    ],
    constructors=[
        PreCompiledTargetConstructor(
            "search",
            [
                (
                    Parameter(doc_id_set_iterator),
                    [
                        lucene90_postings_enum_target,
                        lucene90_impacts_enum_target,
                        impacts_disi_target,
                        block_max_conjunction_scorer_iterator_target
                    ]
                ),
                (
                    Parameter(two_phase_iterator),
                    [
                        PCO_NULL_TYPE
                    ]
                ),
                (
                    Parameter(bits),
                    [
                        PCO_NULL_TYPE
                    ]
                ),
            ],
            [
                Parameter(scorer),
                Parameter(leaf_collector),
            ],
        )
    ]
)

targets.append(default_bulk_scorer_target)

#########
# Main ##
#########

build_config = PreCompiledTargetBuildConfiguration(
    os.path.abspath(os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "..", "..", "..")),
)

gen_pco_sources(build_config, targets)
